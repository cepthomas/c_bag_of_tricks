#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include "common.h"


/// @brief Declaration of state machine.

//---------------- Public API ----------------------//

/// Opaque state machine object.
typedef struct sm sm_t;

/// Transition function type.
/// @return Status code.
typedef int (*func_t)(void);

/// Translate a state or event to readable.
/// @param id The state or event id.
/// @return String version of id. 
typedef const char* (*xlat_t)(unsigned int id);

/// Create a state machine. Client must sm_destroy() it.
/// Currently this is NOT thread-safe.
/// @param fp Optional stream for tracing. Can be NULL if not used.
/// @param xlat Optional translator for id tracing.
/// @param def_state The default state id.
/// @param def_event The default event id.
/// @return The opaque pointer used in all functions | BAD_PTR.
sm_t* sm_Create(FILE* fp, xlat_t xlat, unsigned int def_state, unsigned int def_event);

/// Clean up all resources including the state machine.
/// @param sm Pertinent state machine.
/// @return RS_PASS | RS_ERR.
int sm_Destroy(sm_t* sm);

/// Add a new state - sets to current state.
/// @param sm Pertinent state machine.
/// @param state_id State name.
/// @param func Optional entry function to call.
int sm_AddState(sm_t* sm, unsigned int state_id, const func_t func);

/// Add a transition to the current state.
/// @param sm Pertinent state machine.
/// @param event_id Event that causes the transition.
/// @param func Optional transition function to call.
/// @param next_state State to go to. Can be ST_SAME to stay in the same state.
int sm_AddTransition(sm_t* sm, unsigned int event_id, const func_t func, unsigned int next_state);

/// Process the event in the argument.
/// @param sm Pertinent state machine.
/// @param event_id Specific event id.
/// @return RS_PASS | RS_ERR.
int sm_ProcessEvent(sm_t* sm, unsigned int event_id);

/// Get the current state.
/// @param sm Pertinent state machine.
/// @return Current state id | RS_ERR.
int sm_GetState(sm_t* sm);

/// Reset a machine.
/// @param sm Pertinent state machine.
/// @param state_id State to set to.
/// @return RS_PASS | RS_ERR.
int sm_Reset(sm_t* sm, unsigned int state_id);

/// Dump contents of the loaded state machine as a dot file.
/// @param sm Pertinent state machine.
/// @param fp Output stream.
/// @return RS_PASS | RS_ERR.
int sm_ToDot(sm_t* sm, FILE* fp);

/// Debug logging function.
/// @param sm Pertinent state machine.
/// @param line Line number.
/// @param format Format string followed by args.
/// @return RS_PASS | RS_ERR.
int sm_Trace(sm_t* sm, int line, const char* format, ...);

#endif // STATE_MACHINE_H
