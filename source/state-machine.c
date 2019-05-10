
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "state-machine.h"

/// @file

#define MAX_LOG_LINE 100


/// One transition in a state.
typedef struct
{
    int eventId;      ///< The event to respond to.
    func_t func;      ///< Optional function to execute.
    int nextStateId;  ///< Next state to go to. Can be ST_SAME.
} transDesc_t;

/// One state and its handled events.
typedef struct
{
    int stateId;         ///< Description DOC
    func_t func;         ///< Description DOC
    list_t* transDescs;  ///< Description DOC // transDesc_t
} stateDesc_t;


/// Describes the behavior of a state machine instance.
struct sm
{
    list_t* stateDescs;         ///< All the states - stateDesc_t.
    stateDesc_t* currentState;  ///< The current state.
    stateDesc_t* defaultState;  ///< Maybe a default state.
    struct timeval start;       ///< For measuring elapsed time.
    FILE* fp;                   ///< For logging output - optional.
    xlat_t xlat;                ///< Client supplied translation for logging - optional.
};


//////// Private functions ///////

/// Translate a state or event id to turn into a string.
/// @param sm Pertinent state machine.
/// @param id State or event to translate.
/// @return The translated string.
const char* sm_xlat(sm_t* sm, int id)
{
    switch(id)
    {
        case ST_SAME: return "ST_SAME";
        case ST_DEFAULT: return "ST_DEFAULT";
        case EVT_DEFAULT: return "EVT_DEFAULT";
        default: return (sm->xlat != NULL) ? sm->xlat(id) : "???";
    }
}

//////// Public/client functions ////////

sm_t* sm_create(FILE* fp, xlat_t xlat)
{
    sm_t* sm = malloc(sizeof(sm_t));

    sm->stateDescs = list_create();
    sm->currentState = NULL;
    sm->defaultState = NULL;
    sm->fp = fp;
    sm->xlat = xlat;

    return sm;
}

void sm_destroy(sm_t* sm)
{
    listData_t data;

    list_start(sm->stateDescs);
    while(list_next(sm->stateDescs, &data))
    {
        stateDesc_t* stateDesc = data.p;
        list_t* transDescs = stateDesc->transDescs;
        free(transDescs);
    }
    free(sm->stateDescs);

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
                sm->currentState->func(NULL);//TODO may call sm_processEvent()
            }
        }
    }
}

int sm_getState(sm_t* sm)
{
    return sm->currentState->stateId;
}

const char* sm_getStateString(sm_t* sm)
{
    return sm->xlat(sm->currentState->stateId);
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

    if(stateId == ST_DEFAULT) // keep a reference to this one
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

void sm_processEvent(sm_t* sm, int eventId, void* context)
{
    sm_trace(sm, "Process state %s event %s\n",
             sm_xlat(sm, sm->currentState->stateId), sm_xlat(sm, eventId));

    // Find match with this event for present state.
    listData_t data;
    transDesc_t* transDesc = NULL;
    transDesc_t* defDesc = NULL;

    // Try default state first.
    if(sm->defaultState != NULL)
    {
        list_start(sm->defaultState->transDescs);
        while(list_next(sm->defaultState->transDescs, &data))
        {
            transDesc_t* trans = (transDesc_t*)data.p;
            if(trans->eventId == eventId) // found it
            {
                transDesc = trans;
            }
        }
    }

    // Otherwise check the regulars.
    if(transDesc == NULL)
    {
        list_start(sm->currentState->transDescs);
        while(list_next(sm->currentState->transDescs, &data))
        {
            transDesc_t* trans = (transDesc_t*)data.p;

            if(trans->eventId == eventId) // found it
            {
                transDesc = trans;
            }
            else if(trans->eventId == EVT_DEFAULT)
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
        // Execute the function.
        if(transDesc->func != NULL)
        {
            transDesc->func(context);//TODO may call sm_processEvent()
        }

        // Process the next state.
        if(transDesc->nextStateId != ST_SAME)
        {
            // State is changing. Find the new state.
            stateDesc_t* nextState = NULL;
            list_start(sm->stateDescs);
            while(list_next(sm->stateDescs, &data))
            {
                stateDesc_t* st = (stateDesc_t*)data.p;
                if(st->stateId == transDesc->nextStateId) // found it
                {
                    nextState = st;
                }
            }

            if(nextState != NULL)
            {
                sm_trace(sm, "Changing state from %s to %s\n", sm_xlat(sm,
                       sm->currentState->stateId), sm_xlat(sm, nextState->stateId));
                sm->currentState = nextState;
                if(sm->currentState->func != NULL)
                {
                    sm->currentState->func(context);//TODO may call sm_processEvent()
                }
            }
        }
        else
        {
            sm_trace(sm, "Same state %s\n", sm_xlat(sm, sm->currentState->stateId));
        }
    }
    else
    {
        sm_trace(sm, "No match for state %s for event %s\n",
              sm_xlat(sm, sm->currentState->stateId), sm_xlat(sm, eventId));
    }
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
        
        fprintf(sm->fp, "SM %06f %s", secs, sfmt);
    }
}

void sm_toDot(sm_t* sm)
{
    if(sm->fp != NULL)
    {
        FILE* fp = sm->fp;

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
                        sm_xlat(sm, st->stateId),
                        trans->nextStateId == ST_SAME ? sm_xlat(sm, st->stateId) : sm_xlat(sm, trans->nextStateId),
                        sm_xlat(sm, trans->eventId) );
            }
        }

        fprintf(fp, "}\n");
    }
}
