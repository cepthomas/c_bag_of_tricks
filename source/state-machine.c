
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "state-machine.h"
#include "list.h"


/// @file

#define MAX_LOG_LINE 100


/// One transition in a state.
typedef struct
{
    int eventId;            ///< Unique id for the trigger event.
    func_t func;            ///< Optional function to execute on entry.
    int nextStateId;        ///< Next state to go to. Can be ST_SAME.
} transDesc_t;

/// One state and its handled events.
typedef struct
{
    int stateId;            ///< Unique id for this state.
    func_t func;            ///< Optional function to execute on entry.
    list_t* transDescs;     ///< List of transitions - transDesc_t
} stateDesc_t;

/// Describes the behavior of a state machine instance.
struct sm
{
    // Stuff supplied by client.
    FILE* fp;                   ///< For logging output - optional.
    xlat_t xlat;                ///< Client supplied translation for logging - optional.
    int defState;               ///< The default state id.
    int defEvent;               ///< The default event id.
    // Internal context stuff.
    list_t* stateDescs;         ///< All the states - stateDesc_t.
    stateDesc_t* currentState;  ///< The current state.
    stateDesc_t* defaultState;  ///< Maybe a default state.
    struct timeval start;       ///< For measuring elapsed time.
    list_t* eventQueue;         ///< Queue of events to be processed - int.
    bool processingEvents;      ///< Internal flag for recursion.
};


//////// Public/client functions ////////

sm_t* sm_create(FILE* fp, xlat_t xlat, int defState, int defEvent)
{
    sm_t* sm = malloc(sizeof(sm_t));

    sm->fp = fp;
    sm->xlat = xlat;
    sm->defState = defState;
    sm->defEvent = defEvent;

    sm->stateDescs = list_create();
    sm->currentState = NULL;
    sm->defaultState = NULL;
    sm->eventQueue = list_create();
    sm->processingEvents = false;

    return sm;
}

void sm_destroy(sm_t* sm)
{
    listData_t data1;
    listData_t data2;

    list_start(sm->stateDescs);
    while(list_next(sm->stateDescs, &data1))
    {
        stateDesc_t* stateDesc = data1.p;

        list_start(stateDesc->transDescs);
        while(list_next(stateDesc->transDescs, &data2))
        {
            transDesc_t* transDesc = data2.p;
            free(transDesc);
        }

        list_destroy(stateDesc->transDescs);
        free(stateDesc);
    }
    list_destroy(sm->stateDescs);

    list_destroy(sm->eventQueue);

    free(sm);
}

void sm_reset(sm_t* sm, int stateId)
{
    gettimeofday(&sm->start, NULL);

    listData_t data;
    list_start(sm->stateDescs);
    while(list_next(sm->stateDescs, &data))
    {
        stateDesc_t* st = (stateDesc_t*)data.p;
        if(st->stateId == stateId) // found it
        {
            sm->currentState = st;

            if(sm->currentState->func != NULL)
            {
                sm->currentState->func();
            }
        }
    }
}

int sm_getState(sm_t* sm)
{
    return sm->currentState->stateId;
}

void sm_addState(sm_t* sm, int stateId, const func_t func)
{
    stateDesc_t* stateDesc = malloc(sizeof (stateDesc_t));
    
    stateDesc->stateId = stateId;
    stateDesc->func = func;
    stateDesc->transDescs = list_create();

    listData_t data;
    data.p = stateDesc;
    list_push(sm->stateDescs, data);
    sm->currentState = stateDesc; // for adding transitions

    if(stateId == sm->defState) // keep a reference to this one
    {
        sm->defaultState = stateDesc;
    }
}

void sm_addTransition(sm_t* sm, int eventId, const func_t func, int nextState)
{
    transDesc_t* transDesc = malloc(sizeof (transDesc_t));

    transDesc->eventId = eventId;
    transDesc->func = func;
    transDesc->nextStateId = nextState;

    listData_t data;
    data.p = transDesc;
    list_push(sm->currentState->transDescs, data);
}

void sm_processEvent(sm_t* sm, int eventId)
{
    // Transition functions may generate new events so keep a queue.
    // This allows current execution to complete before handling new event.
    listData_t ld;
    ld.i = eventId;
    list_push(sm->eventQueue, ld);

    // Check for recursion through the processing loop - event may be generated internally during processing.
    if (!sm->processingEvents)
    {
        sm->processingEvents = true;

        // Process all events in the event queue.
        listData_t qevt;
        while (list_pop(sm->eventQueue, &qevt))
        {
            int qevtid = qevt.i;

            sm_trace(sm, "Process current state %s event %s\n",
                     sm->xlat(sm->currentState->stateId), sm->xlat(qevtid));

            // Find match with this event for present state.
            listData_t d;
            transDesc_t* transDesc = NULL;
            transDesc_t* defDesc = NULL;
            //int nextStateId = -1;

            // Try default state first.
            if(sm->defaultState != NULL)
            {
                list_start(sm->defaultState->transDescs);
                while(list_next(sm->defaultState->transDescs, &d))
                {
                    transDesc_t* trans = (transDesc_t*)d.p;
                    if(trans->eventId == qevtid) // found it
                    {
                        transDesc = trans;
                        //nextStateId = transDesc->nextStateId;
                    }
                }
            }

            // Otherwise check the regulars.
            if(transDesc == NULL)
            {
                list_start(sm->currentState->transDescs);
                while(list_next(sm->currentState->transDescs, &d))
                {
                    transDesc_t* trans = (transDesc_t*)d.p;

                    if(trans->eventId == qevtid) // found it
                    {
                        transDesc = trans;
                    }
                    else if(trans->eventId == sm->defEvent)
                    {
                        defDesc = trans;
                    }
                }
            }

            if(transDesc == NULL)
            {
                transDesc = defDesc;
            }

            if(transDesc != NULL)
            {
                // Execute the transition function.
                if(transDesc->func != NULL)
                {
                    transDesc->func();
                }

                // Process the next state.
                if(transDesc->nextStateId != sm->currentState->stateId)
                {
                    // State is changing. Find the new state.
                    stateDesc_t* nextState = NULL;
                    list_start(sm->stateDescs);
                    while(list_next(sm->stateDescs, &d))
                    {
                        stateDesc_t* st = (stateDesc_t*)d.p;
                        if(st->stateId == transDesc->nextStateId) // found it
                        {
                            nextState = st;
                        }
                    }

                    if(nextState != NULL)
                    {
                        sm_trace(sm, "Changing state from %s to %s\n",
                                 sm->xlat(sm->currentState->stateId), sm->xlat(nextState->stateId));
                        sm->currentState = nextState;
                        if(sm->currentState->func != NULL)
                        {
                            sm->currentState->func();
                        }
                    }
                    else
                    {
                        sm_trace(sm, "Couldn't find next state from %s to %s\n",
                                 sm->xlat(sm->currentState->stateId), sm->xlat(nextState->stateId)); //TODO should be an error.
                    }
                }
                else
                {
                    sm_trace(sm, "Same state %s\n", sm->xlat(sm->currentState->stateId));
                }
            }
            else
            {
                sm_trace(sm, "No match for state %s for event %s\n",
                      sm->xlat(sm->currentState->stateId), sm->xlat(qevtid));
            }
        }
    }

    // Done for now.
    sm->processingEvents = false;
}

void sm_trace(sm_t* sm, const char* format, ...)
{
    if(sm->fp != NULL)
    {
        va_list args;
        va_start(args, format);

        char sfmt[MAX_LOG_LINE];
        vsnprintf(sfmt, MAX_LOG_LINE-1, format, args);
        va_end(args);

        // Add elapsed time.
        struct timeval now;
        double secs = 0;

        gettimeofday(&now, NULL);
        secs = (double)(now.tv_usec - sm->start.tv_usec) / 1000000 + (double)(now.tv_sec - sm->start.tv_sec);
        
        fprintf(sm->fp, "SM: %06f %s", secs, sfmt);
    }
}

void sm_toDot(sm_t* sm, FILE* fp)
{
    // Init attributes for dot.
    fprintf(fp, "digraph StateDiagram {\n");
    fprintf(fp, "    ratio=\"compress\";\n");
    fprintf(fp, "    fontname=\"Arial\";\n");
    fprintf(fp, "    label=\"\";\n"); // (your label here!)
    fprintf(fp, "    node [\n");
    fprintf(fp, "    height=\"1.00\";\n");
    fprintf(fp, "    width=\"1.5\";\n");
    fprintf(fp, "    shape=\"ellipse\";\n");
    fprintf(fp, "    fixedsize=\"true\";\n");
    fprintf(fp, "    fontsize=\"8\";\n");
    fprintf(fp, "    fontname=\"Arial\";\n");
    fprintf(fp, "];\n");
    fprintf(fp, "\n");
    fprintf(fp, "    edge [\n");
    fprintf(fp, "    fontsize=\"8\";\n");
    fprintf(fp, "    fontname=\"Arial\";\n");
    fprintf(fp, "];\n");
    fprintf(fp, "\n");

    // Generate actual nodes and edges from states
    // Iterate all states.
    listData_t data;
    list_start(sm->stateDescs);
    while(list_next(sm->stateDescs, &data))
    {
        stateDesc_t* st = (stateDesc_t*)data.p;

        // Iterate through the state transitions.
        list_start(st->transDescs);
        while(list_next(st->transDescs, &data))
        {
            transDesc_t* trans = (transDesc_t*)data.p;

            fprintf(fp, "        \"%s\" -> \"%s\" [label=\"%s\"];\n",
                    sm->xlat(st->stateId),
                    trans->nextStateId == st->stateId ? sm->xlat(st->stateId) : sm->xlat(trans->nextStateId),
                    sm->xlat(trans->eventId) );
        }
    }

    fprintf(fp, "}\n");
}
