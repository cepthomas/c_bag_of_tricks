
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
} lock_data_t;


/// State of the lock.
static bool p_is_locked;

/// Current input being processed.
static char p_current_key;

/// Current combination.
static list_t* p_combination;

/// Where we are in the sequence.
static list_t* p_current_entry;

/// The machine engine.
static sm_t* p_sm;


//////// Transition functions declarations ////////////
/// Clear the lock.
static int ClearCurrentEntry(void);

/// Initialize the lock
static int InitialEnter(void);

/// Add a digit to the current sequence.
static int LockedAddDigit(void);

/// Locked transition function.
static int LockedEnter(void);

/// Try setting a new combination.
static int SetCombo(void);

/// Add a digit to the current sequence.
static int SetComboAddDigit(void);

/// Clear the lock
static int TryDefault(void);

/// Lock is unlocked now.
static int UnlockedEnter(void);


//--------------------------------------------------------//
sm_t* lock_Create(FILE* fp)
{
    p_is_locked = true;
    p_current_entry = list_Create();
    p_combination = list_Create();

    // Initial combination is: 000
    CREATE_INST(k1, lock_data_t, BAD_PTR);
    k1->c = '0';
    list_Append(p_combination, k1);

    CREATE_INST(k2, lock_data_t, BAD_PTR);
    k2->c = '0';
    list_Append(p_combination, k2);

    CREATE_INST(k3, lock_data_t, BAD_PTR);
    k3->c = '0';
    list_Append(p_combination, k3);

    // Build the FSM.
    p_sm = sm_Create(fp, lock_Xlat, ST_DEFAULT, EVT_DEFAULT);
    VAL_PTR(p_sm, BAD_PTR);

    sm_AddState(p_sm, ST_INITIAL,                   InitialEnter);
    sm_AddTransition(p_sm, EVT_IS_LOCKED,           NULL,                   ST_LOCKED);
    sm_AddTransition(p_sm, EVT_IS_UNLOCKED,         NULL,                   ST_UNLOCKED);

    sm_AddState(p_sm, ST_LOCKED,                    LockedEnter);
    sm_AddTransition(p_sm, EVT_DIGIT_KEY_PRESSED,   LockedAddDigit,         ST_LOCKED);
    sm_AddTransition(p_sm, EVT_RESET,               ClearCurrentEntry,      ST_LOCKED);
    sm_AddTransition(p_sm, EVT_VALID_COMBO,         NULL,                   ST_UNLOCKED);
    sm_AddTransition(p_sm, EVT_DEFAULT,             ClearCurrentEntry,      ST_LOCKED);

    sm_AddState(p_sm, ST_UNLOCKED,                  UnlockedEnter);
    sm_AddTransition(p_sm, EVT_RESET,               ClearCurrentEntry,      ST_LOCKED);
    sm_AddTransition(p_sm, EVT_SET_COMBO,           ClearCurrentEntry,      ST_SETTING_COMBO);
    sm_AddTransition(p_sm, EVT_DEFAULT,             ClearCurrentEntry,      ST_UNLOCKED);
 
    sm_AddState(p_sm, ST_SETTING_COMBO,             ClearCurrentEntry);
    sm_AddTransition(p_sm, EVT_DIGIT_KEY_PRESSED,   SetComboAddDigit,       ST_SETTING_COMBO);
    sm_AddTransition(p_sm, EVT_SET_COMBO,           SetCombo,               ST_UNLOCKED);
    sm_AddTransition(p_sm, EVT_RESET,               ClearCurrentEntry,      ST_UNLOCKED);
 
    sm_AddState(p_sm, ST_DEAD,                      NULL);
    // Empty state. Maybe call it ST_SARTRE?

    sm_AddState(p_sm, ST_DEFAULT,                   NULL);
    sm_AddTransition(p_sm, EVT_SHUT_DOWN,           TryDefault,             ST_DEAD);

    // Set our initial state.
    sm_Reset(p_sm, ST_INITIAL);

    return p_sm;
}


//--------------------------------------------------------//
int lock_Destroy(void)
{
    int ret = RS_PASS;

    list_Destroy(p_current_entry);
    list_Destroy(p_combination);

    sm_Destroy(p_sm);

    return ret;
}

//--------------------------------------------------------//
int lock_PressKey(char key)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "Key pressed %c\n", key);

    p_current_key = key;

    switch(key)
    {
        case KEY_RESET:
            ret = sm_ProcessEvent(p_sm, EVT_RESET);
            break;

        case KEY_SET:
            ret = sm_ProcessEvent(p_sm, EVT_SET_COMBO);
            break;

        case KEY_POWER:
            ret = sm_ProcessEvent(p_sm, EVT_SHUT_DOWN);
            break;

        default:
            if(key >= '0' && key <= '9')
            {
                ret = sm_ProcessEvent(p_sm, EVT_DIGIT_KEY_PRESSED);
            }
            // else ignore fat fingers.
            break;
    }

    return ret;
}

//--------------------------------------------------------//
const char* lock_Xlat(unsigned int id)
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
int InitialEnter()
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "InitialEnter()\n");

    if(p_is_locked)
    {
        sm_ProcessEvent(p_sm, EVT_IS_LOCKED);
    }
    else
    {
        sm_ProcessEvent(p_sm, EVT_IS_UNLOCKED);
    }

    return ret;
}

//--------------------------------------------------------//
int LockedEnter(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "LockedEnter()\n");
    p_is_locked = true;
    list_Clear(p_current_entry);

    return ret;
}

//--------------------------------------------------------//
int ClearCurrentEntry(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "ClearCurrentEntry()\n");
    list_Clear(p_current_entry);

    return ret;
}

//--------------------------------------------------------//
int LockedAddDigit(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "LockedAddDigit()\n");

    CREATE_INST(data, lock_data_t, RS_ERR);
    data->c = p_current_key;
    list_Append(p_current_entry, data);

    // Test to see if it matches the stored combination.
    lock_data_t* dcomb;
    lock_data_t* dentry;

    list_IterStart(p_combination);
    list_IterStart(p_current_entry);

    bool ok = list_Count(p_combination) == list_Count(p_current_entry);

    for(int i = 0; i < list_Count(p_combination) && ok; i++)
    {
        list_IterNext(p_combination, (void**)&dcomb);
        list_IterNext(p_current_entry, (void**)&dentry);

        if(dcomb != NULL && dentry != NULL)
        {
            ok = dcomb->c == dentry->c;
        }
    }

    if(ok)
    {
        sm_ProcessEvent(p_sm, EVT_VALID_COMBO);
    }

    return ret;
}

//--------------------------------------------------------//
int SetComboAddDigit(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "SetComboAddDigit()\n");

    CREATE_INST(data, lock_data_t, RS_ERR);
    data->c = p_current_key;
    list_Append(p_current_entry, data);

    return ret;
}

//--------------------------------------------------------//
int SetCombo(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "SetCombo()\n");

    if(list_Count(p_current_entry) > 0)
    {
        list_Clear(p_combination);

        // Copy data over.
        list_IterStart(p_current_entry);
        bool done = false;
        lock_data_t* data;
        while (!done)
        {
            if(list_IterNext(p_current_entry, (void**)&data))
            {
                CREATE_INST(data2, lock_data_t, RS_ERR);
                list_Append(p_combination, data2);
            }
            else
            {
               done = true;
            }
        }

        list_Clear(p_current_entry);
    }

    return ret;
}

//--------------------------------------------------------//
int UnlockedEnter(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "UnlockedEnter()\n");
    p_is_locked = false;

    return ret;
}

//--------------------------------------------------------//
int TryDefault(void)
{
    int ret = RS_PASS;

    sm_Trace(p_sm, __LINE__, "TryDefault()\n");
    p_is_locked = true;
    list_Clear(p_current_entry);

    return ret;
}
