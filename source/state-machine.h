#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include "list.h"


////////// Client interface to state machine //////////

/// Opaque state machine object.
typedef struct sm sm_t;

/// Transition function type. Carries ptr to app specific context data.
typedef void (*func_t)(void*);

/// Translate a state or event to readable.
typedef const char* (*xlat_t)(int);

/// Signifies stay in same state.
#define ST_SAME -1

/// Signifies state for default handlers.
#define ST_DEFAULT -2

/// Signifies default event handler.
#define EVT_DEFAULT -3

/// Create a machine. Pass a debug stream and translate function. Returns opaque pointer.
/// Client must sm_destroy() it.
sm_t* sm_create(FILE* fp, xlat_t xlat);

/// Clean up all resources including the sm.
void sm_destroy(sm_t* sm);

/// Reset a machine.
void sm_reset(sm_t* sm);

/// Add a new state - sets to current state.
void sm_addState(sm_t* sm, int stateId, const func_t func);

/// Add an event to the current state.
void sm_addEvent(sm_t* sm, int eventId, const func_t func, int nextState);

/// Process the event in the argument.
void sm_processEvent(sm_t* sm, int eventId, void* context);

/// Get the current state.
int sm_currentState(sm_t* sm);

/// Dump contents of the loaded state machine to stdout as readable.
void sm_dump(sm_t* sm);

/// Dump contents of the loaded state machine to stdout as a dot file.
void sm_toDot(sm_t* sm);

/// Debug logging function.
void sm_trace(sm_t* sm, const char* format, ...);

#endif // STATE_MACHINE_H
