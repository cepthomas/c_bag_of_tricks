#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "cbot.h"
#include "stringx.h"
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STR_BASIC, "Test basic stringx functions.")
{
    // UT_EQUAL(stringx_Compare(s3, "round and square", true), CBOT_PASS);
    // Basic ops.
    stringx_t* s1 = stringx_Create("  round and square  ");
    UT_NOT_NULL(stringx_Content(s1));
    UT_EQUAL(stringx_Len(s1), 20);
    UT_STR_EQUAL(stringx_Content(s1), "  round and square  ");

    stringx_t* s2 = stringx_Create("");
    UT_NOT_NULL(stringx_Content(s2));
    UT_EQUAL(stringx_Len(s2), 0);
    UT_STR_EQUAL(stringx_Content(s2), "");

    stringx_Set(s2, "The Mulberry Bush");
    UT_NOT_NULL(stringx_Content(s2));
    UT_EQUAL(stringx_Len(s2), 17);
    UT_STR_EQUAL(stringx_Content(s2), "The Mulberry Bush");

    stringx_Trim(s1);
    UT_STR_EQUAL(stringx_Content(s1), "round and square");

    UT_TRUE(stringx_StartsWith(s1, "round", true));
    UT_FALSE(stringx_StartsWith(s1, "Round", true));
    UT_TRUE(stringx_StartsWith(s1, "Round", false));
    UT_FALSE(stringx_StartsWith(s1, "xyz", true));
    UT_FALSE(stringx_StartsWith(s1, "xyz", false));

    UT_FALSE(stringx_EndsWith(s2, "bush", true));
    UT_TRUE(stringx_EndsWith(s2, "Bush", true));
    UT_TRUE(stringx_EndsWith(s2, "bush", false));
    UT_FALSE(stringx_EndsWith(s2, "xyz", true));
    UT_FALSE(stringx_EndsWith(s2, "xyz", false));

    UT_EQUAL(stringx_Contains(s1, "and squ", true), 6);
    UT_EQUAL(stringx_Contains(s1, "anD squ", true), -CBOT_ERR_INVALID_INDEX);
    UT_EQUAL(stringx_Contains(s1, "D squ", false), 8);
    UT_EQUAL(stringx_Contains(s1, "xyz", true), -CBOT_ERR_INVALID_INDEX);
    UT_EQUAL(stringx_Contains(s1, "xyz", false), -CBOT_ERR_INVALID_INDEX);

    UT_EQUAL(stringx_Compare(s1, "round and square", true), 0);
    UT_EQUAL(stringx_Compare(s1, "roUnd and sQuare", false), 0);
    UT_EQUAL(stringx_Compare(s1, "roUnd and sQuare", true), 1);
    UT_EQUAL(stringx_Compare(s1, "xyz", true), 2);
    UT_EQUAL(stringx_Compare(s1, "less", false), 1);

    stringx_ToUpper(s1);
    UT_EQUAL(stringx_Compare(s1, "ROUND AND SQUARE", true), 0);
    stringx_ToLower(s1);
    UT_EQUAL(stringx_Compare(s1, "round and square", true), 0);

    // Copy ops.
    stringx_t* s3 = stringx_Copy(s1);
    UT_NOT_NULL(s3);
    UT_NOT_NULL(stringx_Content(s3));

    stringx_t* s4 = stringx_Left(s2, 6);  //"The Mulberry Bush"
    UT_NOT_NULL(s4);
    UT_STR_EQUAL(stringx_Content(s4), "The Mu");
    UT_STR_EQUAL(stringx_Content(s2), "lberry Bush");

    // Append string.
    stringx_Append(s2, s1);
    UT_STR_EQUAL(stringx_Content(s2), "lberry Bushround and square");

    // Clean up.
    stringx_Destroy(s1);
    stringx_Destroy(s2);
    stringx_Destroy(s3);
    stringx_Destroy(s4);
    // Don't try to destroy the same object twice!
    //stringx_Destroy(s4);

    return 0; // pass
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STR_FANCY, "Test fancier stringx functions.")
{
    stringx_t* s1 = stringx_Create("  round and square  ");
    stringx_t* s2 = stringx_Create("");

    list_t* parts = stringx_Split(s1, " ");
    int cnt = list_Count(parts);
    UT_EQUAL(cnt, 3);
    list_IterStart(parts);
    char* data;
    list_IterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "round");
    list_IterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "and");
    list_IterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "square");

    // Formatting.
    int i = 123;
    double d = 44.99;
    const char* s = "xyzzy oooo";

    UT_EQUAL(stringx_Format(s2, 100, "i:%d I am a GOOD formatted string with d:%f s:%s x:%0X", i, d, s, i), CBOT_ERR_NO_ERR);
    UT_EQUAL(stringx_Compare(s2, "i:123 I am a GOOD formatted string with d:44.990000 s:xyzzy oooo x:7B", true), CBOT_ERR_NO_ERR);

    // Clean up.
    stringx_Destroy(s1);
    stringx_Destroy(s2);
    list_Destroy(parts);

    return 0; // pass
}
