
#ifndef LOCK_H
#define LOCK_H


///// Event IDs
#define EVT_FOO 100
#define EVT_BAR 101
#define EVT_DIGIT_KEY_PRESSED 102
#define EVT_IS_LOCKED 104
#define EVT_IS_UNLOCKED 105
#define EVT_RESET 106
#define EVT_SET_COMBO 107
#define EVT_SHUT_DOWN 108
#define EVT_VALID_COMBO 109

///// State IDs
#define ST_FOO 200
#define ST_BAR 201
#define ST_INITIAL 202
#define ST_LOCKED 203
#define ST_SETTING_COMBO 205
#define ST_UNLOCKED 206


///// Special keys. 0-9 are simply their ascii values.
#define KEY_RESET '*'
#define KEY_SET '#'
#define KEY_POWER '!'


/////// Client interface //////////

/// Normal constructor.
void lock_create(void);

/// Normal destructor.
void lock_destroy(void);

// Input from the keypad.
void lock_pressKey(char key);

//////////// Stuff needed for unit testing TODO hide better ///////////

/// Get the current state.
int lock_currentState(void);

/// Force a condition.
void lock_inject(int eventId);

#endif // LOCK_H
