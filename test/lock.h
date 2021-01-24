
#ifndef LOCK_H
#define LOCK_H

#include <stdio.h>

#include "state_machine.h"

////// Ids - all unique ///////

// Event IDs
#define EVT_DEFAULT 100
#define EVT_DIGIT_KEY_PRESSED 101
#define EVT_IS_LOCKED 102
#define EVT_IS_UNLOCKED 103
#define EVT_RESET 104
#define EVT_SET_COMBO 105
#define EVT_SHUT_DOWN 106
#define EVT_VALID_COMBO 107
#define EVT_FOO 108

// State IDs
#define ST_DEFAULT 200
#define ST_INITIAL 201
#define ST_LOCKED 202
#define ST_SETTING_COMBO 203
#define ST_UNLOCKED 204
#define ST_DEAD 205


///// Special keys. ///////
// 0-9 are simply their ascii values.
#define KEY_RESET '*'
#define KEY_SET '#'
#define KEY_POWER '!'


/////// Public interface //////////

/// Normal constructor.
/// @param fp Stream for tracing.
/// @return Pointer to underlying state machine object.
sm_t* lock_create(FILE* fp);

/// Normal destructor.
void lock_destroy(void);

/// Process input from the keypad.
/// @param key Specific key pressed.
void lock_pressKey(char key);

/// Readable version of defined state or event.
/// @param id State or event id to translate.
/// @return The corresponding string.
const char* lock_xlat(unsigned int id);

#endif // LOCK_H
