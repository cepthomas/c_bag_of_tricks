
// The test suites to use to test the tester. Say 10 times fast.

#include "pnut.h"


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(PNUT_1, "Test basic check macros")
{
    unsigned int seed = 999;
    srand(seed);

    UT_PROPERTY("version", "xyz123");

    UT_PROPERTY("rand-seed", seed);

    int val1 = 1;
    int val2 = 2;

    UT_INFO("Visually inspect that this appears in the output with parm == 2.", val2);

    UT_EQUAL(val1 + val2, 3);

    UT_GREATER(val1, val2);

    void* p = nullptr;
    UT_NULL(p);
    p = malloc(100);
    UT_NOT_NULL(p);
    free(p);
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(PNUT_2, "The remaining tests for pnut.h. Some will fail deliberately.")
{
    int iTest1 = 321;
    int iTest2 = 987;
    const char* zTest1 = "round and round";
    const char* zTest2 = "the mulberry bush";

    UT_FALSE(iTest2 < iTest1);

    UT_TRUE(iTest2 == 987);

    UT_STR_NOT_EQUAL(zTest1, zTest2);

    UT_STR_EQUAL(zTest2, "the mulberry bush");

    UT_EQUAL(iTest1, 321);

    UT_NOT_EQUAL(iTest2, iTest1);

    UT_INFO("Should fail", ">>>");
    UT_LESS_OR_EQUAL(iTest2, iTest1);

    UT_LESS_OR_EQUAL(iTest1, 321);

    UT_LESS_OR_EQUAL(iTest1, iTest2);

    UT_INFO("Should fail", ">>>");
    UT_GREATER(iTest1, iTest2);

    UT_GREATER(iTest2, iTest1);

    UT_INFO("Should fail", ">>>");
    UT_GREATER_OR_EQUAL(iTest1, iTest2);

    UT_GREATER_OR_EQUAL(iTest2, 987);

    UT_INFO("Inspect", "Clouseau");

    UT_CLOSE(1.500, 1.501, 0.001);

    UT_CLOSE(1.500, 1.499, 0.001);

    UT_INFO("Should fail", ">>>");
    UT_CLOSE(1.500, 1.501, 0.0009);

    UT_INFO("Should fail", ">>>");
    UT_CLOSE(1.500, 1.499, 0.0009);
}
