
#include <stdlib.h>
#include "common.h"
#include "list.h"
#include "lock.h"
#include "state_machine.h"


/// The lock is both an example of how to use the state machine and the unit test case.
//////////////////////////////////////////////////////////////////////////////////////

/// List data payload.
typedef struct
{
    char c;     ///< Character.
} lockData_t;


/// State of the lock.
static bool p_isLocked;

/// Current input being processed.
static char p_currentKey;

/// Current combination.
static list_t* p_combination;

/// Where we are in the sequence.
static list_t* p_currentEntry;

/// The machine engine.
static sm_t* p_sm;


//////// Transition functions declarations ////////////
/// Clear the lock.
static void clearCurrentEntry(void);

/// Initialize the lock
static void initialEnter(void);

/// Add a digit to the current sequence.
static void lockedAddDigit(void);

/// Locked transition function.
static void lockedEnter(void);

/// Try setting a new combination.
static void setCombo(void);

/// Add a digit to the current sequence.
static void setComboAddDigit(void);

/// Clear the lock
static void tryDefault(void);

/// Lock is unlocked now.
static void unlockedEnter(void);


//--------------------------------------------------------//
sm_t* lock_create(FILE* fp)
{
    p_isLocked = true;
    p_currentEntry = list_create();
    p_combination = list_create();

    // Initial combination is: 000
    CREATE_INST(k1, lockData_t);
    VALPTR_PTR(k1);
    k1->c = '0';
    list_append(p_combination, k1);

    CREATE_INST(k2, lockData_t);
    VALPTR_PTR(k2);
    k2->c = '0';
    list_append(p_combination, k2);

    CREATE_INST(k3, lockData_t);
    VALPTR_PTR(k3);
    k3->c = '0';
    list_append(p_combination, k3);

    // Build the FSM.
    p_sm = sm_create(fp, lock_xlat, ST_DEFAULT, EVT_DEFAULT);
    VALPTR_PTR(p_sm);

    sm_addState(p_sm, ST_INITIAL,                   initialEnter);
    sm_addTransition(p_sm, EVT_IS_LOCKED,           NULL,                   ST_LOCKED);
    sm_addTransition(p_sm, EVT_IS_UNLOCKED,         NULL,                   ST_UNLOCKED);

    sm_addState(p_sm, ST_LOCKED,                    lockedEnter);
    sm_addTransition(p_sm, EVT_DIGIT_KEY_PRESSED,   lockedAddDigit,         ST_LOCKED);
    sm_addTransition(p_sm, EVT_RESET,               clearCurrentEntry,      ST_LOCKED);
    sm_addTransition(p_sm, EVT_VALID_COMBO,         NULL,                   ST_UNLOCKED);
    sm_addTransition(p_sm, EVT_DEFAULT,             clearCurrentEntry,      ST_LOCKED);

    sm_addState(p_sm, ST_UNLOCKED,                  unlockedEnter);
    sm_addTransition(p_sm, EVT_RESET,               clearCurrentEntry,      ST_LOCKED);
    sm_addTransition(p_sm, EVT_SET_COMBO,           clearCurrentEntry,      ST_SETTING_COMBO);
    sm_addTransition(p_sm, EVT_DEFAULT,             clearCurrentEntry,      ST_UNLOCKED);
 
    sm_addState(p_sm, ST_SETTING_COMBO,             clearCurrentEntry);
    sm_addTransition(p_sm, EVT_DIGIT_KEY_PRESSED,   setComboAddDigit,       ST_SETTING_COMBO);
    sm_addTransition(p_sm, EVT_SET_COMBO,           setCombo,               ST_UNLOCKED);
    sm_addTransition(p_sm, EVT_RESET,               clearCurrentEntry,      ST_UNLOCKED);
 
    sm_addState(p_sm, ST_DEAD,                      NULL);
    // Empty state. Maybe call it ST_SARTRE?

    sm_addState(p_sm, ST_DEFAULT,                   NULL);
    sm_addTransition(p_sm, EVT_SHUT_DOWN,           tryDefault,             ST_DEAD);

    // Set our initial state.
    sm_reset(p_sm, ST_INITIAL);

    return p_sm;
}


//--------------------------------------------------------//
void lock_destroy(void)
{
    list_destroy(p_currentEntry);
    list_destroy(p_combination);

    sm_destroy(p_sm);
}

//--------------------------------------------------------//
void lock_pressKey(char key)
{
    sm_trace(p_sm, __LINE__, "Key pressed %c\n", key);

    p_currentKey = key;

    int ret = RS_PASS;

    switch (key)
    {
        case KEY_RESET:
            ret = sm_processEvent(p_sm, EVT_RESET);
            break;

        case KEY_SET:
            ret = sm_processEvent(p_sm, EVT_SET_COMBO);
            break;

        case KEY_POWER:
            ret = sm_processEvent(p_sm, EVT_SHUT_DOWN);
            break;

        default:
            if(key >= '0' && key <= '9')
            {
                ret = sm_processEvent(p_sm, EVT_DIGIT_KEY_PRESSED);
            }
            // else ignore fat fingers.
            break;
    }

    if(ret != RS_PASS)
    {
        // TODO error - do something.
    }
}

//--------------------------------------------------------//
const char* lock_xlat(unsigned int id)
{
    static char defId[100];

    switch(id)
    {
        case EVT_DEFAULT: return "EVT_DEFAULT";
        case EVT_DIGIT_KEY_PRESSED: return "EVT_DIGIT_KEY_PRESSED";
        case EVT_IS_LOCKED: return "EVT_IS_LOCKED";
        case EVT_IS_UNLOCKED: return "EVT_IS_UNLOCKED";
        case EVT_RESET: return "EVT_RESET";
        case EVT_SET_COMBO: return "EVT_SET_COMBO";
        case EVT_SHUT_DOWN: return "EVT_SHUT_DOWN";
        case EVT_VALID_COMBO: return "EVT_VALID_COMBO";
        case EVT_FOO: return "EVT_FOO";
        case ST_INITIAL: return "ST_INITIAL";
        case ST_DEFAULT: return "ST_DEFAULT";
        case ST_LOCKED: return "ST_LOCKED";
        case ST_SETTING_COMBO: return "ST_SETTING_COMBO";
        case ST_UNLOCKED: return "ST_UNLOCKED";
        case ST_DEAD: return "ST_DEAD";
        default: sprintf(defId, "*** Invalid id %d", id); return defId; // Should be an error?
    }
}

//////// Transition functions ////////////

//--------------------------------------------------------//
void initialEnter()
{
    sm_trace(p_sm, __LINE__, "initialEnter()\n");

    if (p_isLocked)
    {
        sm_processEvent(p_sm, EVT_IS_LOCKED);
    }
    else
    {
        sm_processEvent(p_sm, EVT_IS_UNLOCKED);
    }
}

//--------------------------------------------------------//
static void lockedEnter(void)
{
    sm_trace(p_sm, __LINE__, "lockedEnter()\n");
    p_isLocked = true;
    list_clear(p_currentEntry);
}

//--------------------------------------------------------//
static void clearCurrentEntry(void)
{
    sm_trace(p_sm, __LINE__, "clearCurrentEntry()\n");
    list_clear(p_currentEntry);
}

//--------------------------------------------------------//
static void lockedAddDigit(void)
{
    sm_trace(p_sm, __LINE__, "lockedAddDigit()\n");

    CREATE_INST(data, lockData_t);
    data->c = p_currentKey;
    list_append(p_currentEntry, data);

    // Test to see if it matches the stored combination.
    lockData_t* dcomb;
    lockData_t* dentry;

    list_iterStart(p_combination);
    list_iterStart(p_currentEntry);

    bool ok = list_count(p_combination) == list_count(p_currentEntry);

    for(int i = 0; i < list_count(p_combination) && ok; i++)
    {
        list_iterNext(p_combination, (void**)&dcomb);
        list_iterNext(p_currentEntry, (void**)&dentry);

        if(dcomb != NULL && dentry != NULL)
        {
            ok = dcomb->c == dentry->c;
        }
    }

    if (ok)
    {
        sm_processEvent(p_sm, EVT_VALID_COMBO);
    }
}

//--------------------------------------------------------//
static void setComboAddDigit(void)
{
    sm_trace(p_sm, __LINE__, "setComboAddDigit()\n");

    CREATE_INST(data, lockData_t);
    data->c = p_currentKey;
    list_append(p_currentEntry, data);
}

//--------------------------------------------------------//
static void setCombo(void)
{
    sm_trace(p_sm, __LINE__, "setCombo()\n");

    if (list_count(p_currentEntry) > 0)
    {
        list_clear(p_combination);

        // Copy data over.
        list_iterStart(p_currentEntry);
        bool done = false;
        lockData_t* data;
        while (!done)
        {
            if(list_iterNext(p_currentEntry, (void**)&data))
            {
                CREATE_INST(data2, lockData_t);
                list_append(p_combination, data2);
            }
            else
            {
               done = true;
            }
        }

        list_clear(p_currentEntry);
    }
}

//--------------------------------------------------------//
static void unlockedEnter(void)
{
    sm_trace(p_sm, __LINE__, "unlockedEnter()\n");
    p_isLocked = false;
}

//--------------------------------------------------------//
static void tryDefault(void)
{
    sm_trace(p_sm, __LINE__, "tryDefault()\n");
    p_isLocked = true;
    list_clear(p_currentEntry);
}
