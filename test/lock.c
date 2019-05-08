
#include "lock.h"
#include "state-machine.h"
#include "list.h"


/// The lock is both an example of how to use the state machine and the unit test case.
//////////////////////////////////////////////////////////////////////////////////////


/// State of the lock
static bool s_isLocked;

/// Current combination.
static list_t* s_combination;

/// Where we are in the sequence.
static list_t* s_currentEntry;

/// The machine engine.
static sm_t* s_sm;

/// Data context.
typedef struct
{
    char key;
} lock_context_t;

//////// Transition functions declarations ////////////
static void clearCurrentEntry(void*);
static void initialEnter(void*);
static void lockedAddDigit(void*);
static void lockedEnter(void*);
static void setCombo(void*);
static void setComboAddDigit(void*);
static void tryDefault(void*);
static void unlockedEnter(void*);
static void goBoom(void*);

////// Internal declarations /////////////
static const char* xlat(int id);


/// Normal constructor.
void lock_create(void)
{
    s_isLocked = true;
    s_currentEntry = list_create();
    s_combination = list_create();

    // initial combination is: 000
    listData_t k1; k1.c = '0';
    listData_t k2; k2.c = '0';
    listData_t k3; k3.c = '0';
    list_append(s_combination, k1);
    list_append(s_combination, k2);
    list_append(s_combination, k3);

    // Create the FSM.
    s_sm = sm_create(stdout, xlat);

    sm_addState(s_sm, ST_INITIAL,                 initialEnter);
    sm_addEvent(s_sm, EVT_IS_LOCKED,              NULL,                   ST_LOCKED);
    sm_addEvent(s_sm, EVT_IS_UNLOCKED,            NULL,                   ST_UNLOCKED);

    sm_addState(s_sm, ST_LOCKED,                  lockedEnter);
    sm_addEvent(s_sm, EVT_DIGIT_KEY_PRESSED,      lockedAddDigit,         ST_SAME);
    sm_addEvent(s_sm, EVT_RESET,                  clearCurrentEntry,      ST_SAME);
    sm_addEvent(s_sm, EVT_VALID_COMBO,            NULL,                   ST_UNLOCKED);
    sm_addEvent(s_sm, EVT_DEFAULT,                clearCurrentEntry,      ST_SAME); // ignore other events

    sm_addState(s_sm, ST_UNLOCKED,                unlockedEnter);
    sm_addEvent(s_sm, EVT_RESET,                  clearCurrentEntry,      ST_LOCKED);
    sm_addEvent(s_sm, EVT_SET_COMBO,              clearCurrentEntry,      ST_SETTING_COMBO);
    sm_addEvent(s_sm, EVT_DEFAULT,                clearCurrentEntry,      ST_SAME); // ignore other events
 
    sm_addState(s_sm, ST_SETTING_COMBO,           clearCurrentEntry);
    sm_addEvent(s_sm, EVT_DIGIT_KEY_PRESSED,      setComboAddDigit,       ST_SAME);
    sm_addEvent(s_sm, EVT_SET_COMBO,              setCombo,               ST_UNLOCKED);
    sm_addEvent(s_sm, EVT_RESET,                  clearCurrentEntry,      ST_UNLOCKED);
 
    sm_addState(s_sm, ST_DEFAULT,                 NULL);
    sm_addEvent(s_sm, EVT_SHUT_DOWN,              tryDefault,             ST_LOCKED);
    sm_addEvent(s_sm, EVT_BAR,                    goBoom,                 ST_BAR);
    sm_addEvent(s_sm, EVT_FOO,                    NULL,                   ST_SAME);

    // Set our initial state.
    sm_reset(s_sm);
}

//////// Interface functions definitions /////////

void lock_destroy(void)
{
    free(s_currentEntry);
    free(s_combination);

    sm_destroy(s_sm);
}

int lock_currentState(void)
{
    return sm_currentState(s_sm);
}

void lock_inject(int eventId)
{
    sm_processEvent(s_sm, eventId, NULL);
}

void lock_pressKey(char key)
{
    sm_trace(s_sm, "KeyPressed:%c", key);

    lock_context_t* ctxt = malloc(sizeof(lock_context_t));
    ctxt->key = key;

    switch (key)
    {
        case KEY_RESET:
            sm_processEvent(s_sm, EVT_RESET, ctxt);
            break;

        case KEY_SET:
            sm_processEvent(s_sm, EVT_SET_COMBO, ctxt);
            break;

        case KEY_POWER:
            sm_processEvent(s_sm, EVT_SHUT_DOWN, ctxt);
            break;

        default:
            if(key >= '0' && key <= '9')
            {
                sm_processEvent(s_sm, EVT_DIGIT_KEY_PRESSED, ctxt);
            }
            // else ignore fat fingers.
            break;
    }

    free(ctxt);
}

/// Readable version of current state or event.
const char* lock_xlat(int id)
{
    static char defId[30];

    switch(id)
    {
        case EVT_FOO: return "EVT_FOO";
        case EVT_BAR: return "EVT_BAR";
        case EVT_DIGIT_KEY_PRESSED: return "EVT_DIGIT_KEY_PRESSED";
        case EVT_IS_LOCKED: return "EVT_IS_LOCKED";
        case EVT_IS_UNLOCKED: return "EVT_IS_UNLOCKED";
        case EVT_RESET: return "EVT_RESET";
        case EVT_SET_COMBO: return "EVT_SET_COMBO";
        case EVT_SHUT_DOWN: return "EVT_SHUT_DOWN";
        case EVT_VALID_COMBO: return "EVT_VALID_COMBO";
        case ST_FOO: return "ST_FOO";
        case ST_BAR: return "ST_BAR";
        case ST_INITIAL: return "ST_INITIAL";
        case ST_LOCKED: return "ST_LOCKED";
        case ST_SETTING_COMBO: return "ST_SETTING_COMBO";
        case ST_UNLOCKED: return "ST_UNLOCKED";
        default: sprintf(defId, "NG ID %d", id); return defId;
    }
}

//////// Transition functions ////////////

/// Initialize the lock
void initialEnter(void* c)
{
    sm_trace(s_sm, "InitialEnter");
    lock_context_t* ctxt = (lock_context_t*)c;

    if (s_isLocked)
    {
        sm_processEvent(s_sm, EVT_IS_LOCKED, ctxt);
    }
    else
    {
        sm_processEvent(s_sm, EVT_IS_UNLOCKED, ctxt);
    }
}

/// Locked transition function.
static void lockedEnter(void* c)
{
    (void)c;
    sm_trace(s_sm, "LockedEnter");
    s_isLocked = true;
    list_clear(s_currentEntry);
}

/// Clear the lock.
static void clearCurrentEntry(void* c)
{
    (void)c;
    sm_trace(s_sm, "ClearCurrentEntry");
    list_clear(s_currentEntry);
}

/// Add a digit to the current sequence.
static void lockedAddDigit(void* c)
{
    sm_trace(s_sm, "LockedAddDigit");
    lock_context_t* ctxt = (lock_context_t*)c;

    listData_t data;
    data.c = ctxt->key;
    list_append(s_currentEntry, data);

    // Test to see if it matches the stored combination.
    // quick check
    bool ok = list_count(s_combination) == list_count(s_currentEntry);

    listData_t dcomb;
    listData_t dentry;
    list_start(s_combination);
    list_start(s_currentEntry);

    for(int i = 0; i < list_count(s_combination) && ok; i++)
    {
        list_next(s_combination, &dcomb);
        list_next(s_currentEntry, &dentry);
        ok = dcomb.c == dentry.c;
    }

    if (ok)
    {
        sm_processEvent(s_sm, EVT_VALID_COMBO, ctxt);
    }
}

/// Add a digit to the current sequence.
static void setComboAddDigit(void* c)
{
    sm_trace(s_sm, "SetComboAddDigit");
    lock_context_t* ctxt = (lock_context_t*)c;

    listData_t data;
    data.c = ctxt->key;
    list_append(s_currentEntry, data);
}

/// Try setting a new combination.
static void setCombo(void* c)
{
    (void)c;
    sm_trace(s_sm, "SetCombo");

    if (list_count(s_currentEntry) > 0)
    {
        list_clear(s_combination);

        // Copy data over.
        list_start(s_currentEntry);
        bool done = false;
        while (!done)
        {
            listData_t data;
            if(list_next(s_currentEntry, &data))
            {
                list_append(s_combination, data);
            }
            else
            {
               done = true;
            }
        }

        list_clear(s_currentEntry);
    }
}

/// Lock is unlocked now.
static void unlockedEnter(void* c)
{
    (void)c;
    sm_trace(s_sm, "UnlockedEnter");
    s_isLocked = false;
}

/// Clear the lock
static void tryDefault(void* c)
{
    (void)c;
    sm_trace(s_sm, "ClearCurrentEntry");
    s_isLocked = true;
    list_clear(s_currentEntry);
}

/// For testing.
static void goBoom(void* c)
{
    sm_processEvent(s_sm, 99999, c);

}
