
#include <unistd.h>
#include "pnut.h"

extern "C"
{
#include "lock.h"
}

#define STATE_STR lock_xlat(sm_getState(sm))


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(SM_MAIN, "Test the full state machine using a real world example.")
{
    // Create a new lock.
    sm_t* sm = lock_create(nullptr); // stdout);

    // Should come up in the locked state.
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // Enter the default combination of 000.
    lock_pressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    lock_pressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    lock_pressKey('0');

    // Should now be unlocked.
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Test the default handler. Should stay in the same state.
    lock_pressKey('5');
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Lock it again.
    lock_pressKey(KEY_RESET);
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // Unlock it again.
    lock_pressKey('0');
    lock_pressKey('0');
    lock_pressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Must be in the unlocked state to change the combination.
    // Press set, new combo, set, set the combination to 123.
    lock_pressKey(KEY_SET);
    UT_STR_EQUAL(STATE_STR, "ST_SETTING_COMBO");

    UT_STR_EQUAL(STATE_STR, "ST_SETTING_COMBO");

    lock_pressKey('1');
    lock_pressKey('2');
    lock_pressKey('3');
    UT_STR_EQUAL(STATE_STR, "ST_SETTING_COMBO");

    lock_pressKey(KEY_SET);

    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Default state test.
    lock_pressKey(KEY_POWER);

    UT_STR_EQUAL(STATE_STR, "ST_DEAD");

    // Clean up.
    lock_destroy();
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(SM_DOT, "Test the dot file creation.")
{
    FILE* fp = fopen("sm.gv", "w");
    UT_NOT_NULL(fp);

    // Create a new lock.
    sm_t* sm = lock_create(nullptr);

    sm_toDot(sm, fp);

    int r = system("dot -Tpng sm.gv -o sm.png"); //TODO this doesn't work???
    UT_EQUAL(r, 0);

    // Clean up.
    fclose(fp);

    lock_destroy();
}
