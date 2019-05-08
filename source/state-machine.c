
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "state-machine.h"


/// One event handler in a state.
typedef struct
{
    int eventId;
    func_t func;
    int nextStateId;
} eventDesc_t;

/// One state and its handled events.
typedef struct
{
    int stateId;
    func_t func;
    list_t* eventDescs; // eventDesc_t
} stateDesc_t;


/// Describes the behavior of a state machine instance.
struct sm
{
    /// All the states - stateDesc_t.
    list_t* stateDescs;

    /// The current state.
    stateDesc_t* currentState;

    /// Maybe a default state.
    stateDesc_t* defaultState;

    /// For measuring elapsed time.
    struct timeval start;

    /// For logging output.
    FILE* fp;

    /// For logging support.
    xlat_t xlat;
};

#define ST_SAME -1

/// Signifies state for default handlers.
#define ST_DEFAULT -2

/// Signifies default event handler.
#define EVT_DEFAULT -3

//////// Private functions ///////

const char* xlat(sm_t* sm, int id)
{
    switch(id)
    {
        case ST_SAME: return "ST_SAME";
        case ST_DEFAULT: return "ST_DEFAULT";
        case EVT_DEFAULT: return "EVT_DEFAULT";
        default: if(sm->xlat != NULL) return sm->xlat(id); else return "???";
    }
}

//////// Client functions ////////

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
        list_t* eventDescs = data.p;
        list_start(eventDescs);
        while(list_next(eventDescs, &data))
        {
            free(eventDescs);
        }

        free(sm->stateDescs);
    }

    free(sm);
}

void sm_reset(sm_t* sm)
{
    listData_t data;
    if(list_first(sm->stateDescs, &data))
    {
        sm->currentState = (stateDesc_t*)data.p;
    }
    gettimeofday(&sm->start, NULL);
}

void sm_addState(sm_t* sm, int stateId, const func_t func)
{
    stateDesc_t* stateDesc = malloc(sizeof (stateDesc_t));
    
    stateDesc->stateId = stateId;
    stateDesc->func = func;

    if(stateId == ST_DEFAULT) // special handling
    {
        sm->defaultState = stateDesc;
    }
    else
    {
        listData_t data;
        data.p = stateDesc;
        list_push(sm->stateDescs, data);

        sm->currentState = stateDesc;
    }
}

void sm_addEvent(sm_t* sm, int eventId, const func_t func, int nextState)
{
    eventDesc_t* eventDesc = malloc(sizeof (eventDesc_t));

    eventDesc->eventId = eventId;
    eventDesc->func = func;
    eventDesc->nextStateId = nextState;

    listData_t data;
    data.p = eventDesc;
    list_push(sm->currentState->eventDescs, data);
}

int sm_currentState(sm_t* sm)
{
    return sm->currentState->stateId;
}

#define MAX_LOG_LINE 100
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

void sm_processEvent(sm_t* sm, int eventId, void* context)
{
   // /// Lazy init first.
   // if (!mbInited)
   // {
   //     for (int mi = 0; mi < s_states.size(); mi++)
   //     {
   //         stateDesc_t& map = s_states[mi];
   //         for (int ei = 0; ei < map.EventResponses.size(); ei++)
   //         {
   //             eventDesc_t& er = map.eventDescs[ei];
   //             // Find the matching state, ignoring default for now. Same state are kept intact.
   //             for (int j = 1; j < s_states.size(); j++)
   //             {
   //                 if (s_states[j].State.Match(er.NextState))
   //                     if (er.NextState.iIndex != SAME_STATE)
   //                         er.NextState.iIndex = j;
   //             }

   //             if (er.NextState.iIndex == ST_INVALID) // Means it wasn't found so use the default.
   //                 er.NextState.iIndex = 0;
   //         }
   //     }

   //     mbInited = CMN_TRUE;
   //     s_currentState = 1;
   // }

   sm_trace(sm, "Process event %s", xlat(sm, eventId));


   //   listData_t data;
   //   list_start(sm->stateDescs);
   //   while(list_next(sm->stateDescs, &data))
   //   {
   //       list_t* eventDescs = data.p;
   //       list_start(eventDescs);
   //       while(list_next(eventDescs, &data))
   //       {
   //           free(eventDescs);
   //       }
   //       free(sm->stateDescs);
   //   }



    // Find match with this event for present state          .

    listData_t data;
    eventDesc_t* eventDesc = NULL;
    eventDesc_t* defDesc = NULL;

    // Try default state first.
    if(sm->defaultState != NULL)
    {
        list_start(sm->defaultState->eventDescs);
        while(list_next(sm->defaultState->eventDescs, &data))
        {
            eventDesc_t* evt = (eventDesc_t*)data.p;
            if(evt->eventId == eventId) // found it
            {
                eventDesc = evt;
            }
        }
    }

    // Then the regulars.
    if(eventDesc != NULL)
    {
        list_start(sm->currentState->eventDescs);
        while(list_next(sm->currentState->eventDescs, &data))
        {
            eventDesc_t* evt = (eventDesc_t*)data.p;

            if(evt->eventId == eventId) // found it
            {
                eventDesc = evt;
            }
            else if(evt->eventId == EVT_DEFAULT)
            {
                defDesc = evt;
            }
        }
    }

    if(eventDesc == NULL)
    {
        eventDesc = defDesc;
    }

    if(eventDesc != NULL)
    {
        // Execute the function.
        if(eventDesc->func != NULL)
        {
            eventDesc->func(context);
        }

        // Process the next state.
        if(eventDesc->nextStateId != ST_SAME)
        {
            // Changing. Find the new state.
            stateDesc_t* nextState = NULL;
            list_start(sm->stateDescs);
            while(list_next(sm->stateDescs, &data))
            {
                stateDesc_t* st = (stateDesc_t*)data.p;
                if(st->stateId == eventDesc->nextStateId) // found it
                {
                    nextState = st;
                }
            }

            if(nextState != NULL)
            {
                sm_trace(sm, "Changing state from %s to %s", xlat(sm, sm->currentState->stateId), xlat(sm, nextState->stateId));
                sm->currentState = nextState;
                if(sm->currentState->func != NULL)
                {
                    sm->currentState->func(context);
                }
            }
        }
        else
        {
            sm_trace(sm, "Same state %s", xlat(sm, sm->currentState->stateId));
        }
    }
    else
    {
        sm_trace(sm, "No match for state %s for event %s", xlat(sm, sm->currentState->stateId), xlat(sm, eventId));
    }
}

void sm_dump(sm_t* sm)
{
    // int RetStat = int_NO_ERR;
    // TLOG_CONTEXT_S(CMN_CStateMachine::sm_toString();

    // CMN_CHAR zBuff[MAX_LOG_LINE];
    // snprintf(zBuff, MAX_LOG_LINE, "State machine table s_currentState <%d>", s_currentState);
    // sm_trace(sm, zBuff);

    // for (int i = 0; i < s_states.size(); i++)
    // {
    //    if (i == 0)
    //        sprintf(zBuff, "stateDesc_t for <DEFAULT>");
    //    else
    //        sprintf(zBuff, "stateDesc_t for <%s>", s_states[i].State.sm_toString()));
    //    sm_trace(sm, zBuff);

    //    for (int j = 0; j < s_states[i].EventResponses.size(); j++)
    //    {
    //        sprintf(zBuff, "  eventDesc_t event <%s> func <%0X> next state <%s> index <%d>",
    //                s_states[i].EventResponses[j].Event.sm_toString()),
    //                s_states[i].EventResponses[j].Func,
    //                s_states[i].EventResponses[j].NextState.sm_toString()),
    //                s_states[i].EventResponses[j].NextState.iIndex);
    //        sm_trace(sm, zBuff);
    //    }
    // }
}

void sm_toDot(sm_t* sm)
{
    // doesn't handle C style comments.
    // doesn't deal with send events in subroutines. 
    // add comments to state machine?
    
    //>>>>>public string GenerateDot()
//     List<string> ls = new List<string>
//     {
//         "digraph StateDiagram {",

//         // Init attributes for dot.
//         "    ratio=\"compress\";",
//         "    fontname=\"Arial\";",
//         "    label=\"\";", // (your label here!)
//         "    node [",
//         "    height=\"0.50\";",
//         "    width=\"1.0\";",
//         "    shape=\"ellipse\";",
//         "    fixedsize=\"true\";",
//         "    fontsize=\"8\";",
//         "    fontname=\"Arial\";",
//         "];",
//         "",
//         "    edge [",
//         "    fontsize=\"8\";",
//         "    fontname=\"Arial\";",
//         "];",
//         ""
//     };

//     // Generate actual nodes and edges from states
//     foreach (State s in _states.Values)
//     {
//         // Write a node for the state.
//         //ls.Add($"    \"{s.StateName}\";");

//         // Iterate through the state transitions.
//         foreach (KeyValuePair<string, Transition> kvp in s.Transitions)
//         {
//             Transition t = kvp.Value;

//             // Get event name, but strip off "Transition" suffix if present to save space.
// //                string transitionSuffix = "Transition";
//             string eventName = t.EventName;
//             //if (eventName.EndsWith(transitionSuffix))
//             //{
//             //    eventName = eventName.Substring(0, eventName.Length - transitionSuffix.Length);
//             //}

//             // Write an edge for the transition
//             string nextState = t.NextState;
//             //if (nextState == "SAME_STATE")
//             //{
//             //    nextState = s.StateName;
//             //}
//             ls.Add($"        \"{s.StateName}\" -> \"{nextState}\" [label=\"{eventName}\"];");
//         }

//         //ls.Add("{0}");
//     }

    // ls.Add("}");

    // return string.Join(Environment.NewLine, ls);

}
