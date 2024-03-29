
#include <sys/stat.h>

#include "pnut.h"

extern "C"
{
#include "logger.h"
#include "cbot.h"
#include "lock.h"
}

#define STATE_STR lock_Xlat(sm_GetState(sm))


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(SM_MAIN, "Test the full state machine using a real world example.")
{
    logger_SetFilters(LVL_DEBUG);

    // Create a new lock.
    sm_t* sm = lock_Create();

    // Should come up in the locked state.
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // Enter the default combination of 000.
    lock_PressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    lock_PressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    lock_PressKey('0');
    // Should now be unlocked.
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Test the default handler. Should stay in the same state.
    lock_PressKey('5');
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Lock it again.
    lock_PressKey(KEY_RESET);
    UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // Unlock it again.
    lock_PressKey('0');
    lock_PressKey('0');
    lock_PressKey('0');
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Must be in the unlocked state to change the combination.
    // Press set, new combo, set, set the combination to 123.
    lock_PressKey(KEY_SET);
    UT_STR_EQUAL(STATE_STR, "ST_SETTING_COMBO");

    lock_PressKey('1');
    lock_PressKey('2');
    lock_PressKey('3');
    UT_STR_EQUAL(STATE_STR, "ST_SETTING_COMBO");

    lock_PressKey(KEY_SET);
    UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // Default state test.
    lock_PressKey(KEY_POWER);
    UT_STR_EQUAL(STATE_STR, "ST_DEAD");

    // Clean up.
    lock_Destroy();

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(SM_DOT, "Test the dot file creation.")
{
    logger_SetFilters(LVL_DEBUG);

    system("del /q sm.* >NUL");

    FILE* fp = fopen("..\\test_code\\files\\sm.gv", "w");
    UT_NOT_NULL(fp);

    // Create a lock sm.
    sm_t* sm = lock_Create();
    LOG_DEBUG("Lock created");

    sm_ToDot(sm, fp);
    fclose(fp);

    // Convert to image.
    const char* cmd = "dot -Tpng ..\\test_code\\files\\sm.gv -o sm.png";
    int r = system(cmd);
    UT_EQUAL(r, 0);

    struct stat buffer;
    int ifp = stat("sm.png", &buffer);
    UT_EQUAL(ifp, 0);
    UT_EQUAL(buffer.st_size, 40985);

    // Clean up.
    lock_Destroy();

    return 0;
}
