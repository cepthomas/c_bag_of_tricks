
#ifndef LOCK_H
#define LOCK_H

#include <stdio.h>

////// Ids - all unique ///////

// Event IDs
#define EVT_FOO 100
#define EVT_BAR 101
#define EVT_DIGIT_KEY_PRESSED 102
#define EVT_IS_LOCKED 104
#define EVT_IS_UNLOCKED 105
#define EVT_RESET 106
#define EVT_SET_COMBO 107
#define EVT_SHUT_DOWN 108
#define EVT_VALID_COMBO 109

// State IDs
#define ST_INITIAL 202
#define ST_LOCKED 203
#define ST_SETTING_COMBO 205
#define ST_UNLOCKED 206


///// Special keys. ///////
// Note that 0-9 are simply their ascii values.
#define KEY_RESET '*'
#define KEY_SET '#'
#define KEY_POWER '!'


/////// Client interface //////////

/// Normal constructor.
void lock_create(FILE* fp);

/// Normal destructor.
void lock_destroy(void);

// Input from the keypad.
void lock_pressKey(char key);

/// Get the state machine for unit testing. TODO hide.
#include "state-machine.h"
sm_t* lock_getSm(void);


#endif // LOCK_H
