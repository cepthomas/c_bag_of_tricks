
#include <sys/stat.h>

#include "pnut.h"

extern "C"
{
#include "logger.h"
#include "cli.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(CLI_MAIN, "TODO-DBG tests please.")
{
    logger_SetFilters(LVL_DEBUG, CAT_ALL);

    // // Should come up in the locked state.
    // UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // // Enter the default combination of 000.
    // lock_PressKey('0');
    // UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    // lock_PressKey('0');
    // UT_STR_EQUAL(STATE_STR, "ST_LOCKED");
    // lock_PressKey('0');
    // // Should now be unlocked.
    // UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // // Test the default handler. Should stay in the same state.
    // lock_PressKey('5');
    // UT_STR_EQUAL(STATE_STR, "ST_UNLOCKED");

    // // Lock it again.
    // lock_PressKey(KEY_RESET);
    // UT_STR_EQUAL(STATE_STR, "ST_LOCKED");

    // // Clean up.
    // lock_Destroy();

    return 0;
}
