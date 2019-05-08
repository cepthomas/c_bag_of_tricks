
#include "pnut.h"

extern "C"
{
#include "lock.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(SM_1, "Test the full StateMachine using a real world example.")
{
    // Create a new lock.
    lock_create();

    //CombinationLock mainDoorLock = new CombinationLock(CombinationLock.HwLockStates.HwIsLocked);
    //LOCK_InitStateMachine();

    // Should come up in the locked state.
    UT_EQUAL(lock_currentState(), ST_LOCKED);

    // Enter the default combination of 000.
    lock_pressKey('0');
    UT_EQUAL(lock_currentState(), ST_LOCKED);
    lock_pressKey('0');
    UT_EQUAL(lock_currentState(), ST_LOCKED);
    lock_pressKey('0');

    // Should now be unlocked.
    UT_EQUAL(lock_currentState(), ST_UNLOCKED);

    // Test the default handler. Should stay in the same state.
    lock_pressKey('5');
    UT_EQUAL(lock_currentState(), ST_UNLOCKED);

    // Lock it again.
    lock_pressKey(KEY_RESET);
    UT_EQUAL(lock_currentState(), ST_LOCKED);

    // Unlock it again.
    lock_pressKey('0');
    lock_pressKey('0');
    lock_pressKey('0');
    UT_EQUAL(lock_currentState(), ST_UNLOCKED);

    // Must be in the unlocked state to change the combination.
    // Press set, new combo, set, set the combination to 123.
    lock_pressKey(KEY_SET);
    UT_EQUAL(lock_currentState(), ST_SETTING_COMBO);

    UT_EQUAL(lock_currentState(), ST_SETTING_COMBO);

    // The state machine is now dead and will no longer process events.??

    lock_pressKey('1');
    lock_pressKey('2');
    lock_pressKey('3');
    UT_EQUAL(lock_currentState(), ST_SETTING_COMBO);

    lock_pressKey(KEY_SET);

    UT_EQUAL(lock_currentState(), ST_UNLOCKED);

    // Default state test.
    lock_pressKey(KEY_POWER);

    UT_EQUAL(lock_currentState(), ST_LOCKED);

    // Make a picture. TODO
    // string sdot = LOCK_SM.GenerateDot();
    // File.WriteAllText("testout.gv", sdot);
    // Process p = new Process();
    // p.StartInfo.FileName = "dot";
    // p.StartInfo.Arguments = "-Tpng testout.gv -o testout.png";
    // bool ok = p.Start();


    // NGEVENT
    lock_inject(EVT_BAR);

    lock_inject(9999);

    // Clean up.
    lock_destroy();
}

