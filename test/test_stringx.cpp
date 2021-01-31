#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "list.h"
#include "stringx.h"
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STR_BASIC, "Test basic stringx functions.")
{
    // Basic ops.
    stringx_t* s1 = stringx_create("  round and square  ");
    UT_NOT_NULL(stringx_content(s1));
    UT_EQUAL(stringx_len(s1), 20);
    UT_STR_EQUAL(stringx_content(s1), "  round and square  ");

    stringx_t* s2 = stringx_create("");
    UT_NOT_NULL(stringx_content(s2));
    UT_EQUAL(stringx_len(s2), 0);
    UT_STR_EQUAL(stringx_content(s2), "");

    stringx_set(s2, "The Mulberry Bush");
    UT_NOT_NULL(stringx_content(s2));
    UT_EQUAL(stringx_len(s2), 17);
    UT_STR_EQUAL(stringx_content(s2), "The Mulberry Bush");

    stringx_trim(s1);
    UT_STR_EQUAL(stringx_content(s1), "round and square");

    UT_EQUAL(stringx_startswith(s1, "round", CASE_SENS), RS_PASS);
    UT_EQUAL(stringx_startswith(s1, "Round", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_startswith(s1, "Round", CASE_INSENS), RS_PASS);
    UT_EQUAL(stringx_startswith(s1, "xxx", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_startswith(s1, "xxx", CASE_INSENS), RS_FAIL);

    UT_EQUAL(stringx_endswith(s2, "bush", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_endswith(s2, "Bush", CASE_SENS), RS_PASS);
    UT_EQUAL(stringx_endswith(s2, "bush", CASE_INSENS), RS_PASS);
    UT_EQUAL(stringx_endswith(s2, "xxx", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_endswith(s2, "xxx", CASE_INSENS), RS_FAIL);

    UT_EQUAL(stringx_contains(s1, "and squ", CASE_SENS), 6);
    UT_EQUAL(stringx_contains(s1, "anD squ", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_contains(s1, "D squ", CASE_INSENS), 8);
    UT_EQUAL(stringx_contains(s1, "xxx", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_contains(s1, "xxx", CASE_INSENS), RS_FAIL);

    UT_EQUAL(stringx_compare(s1, "round and square", CASE_SENS), RS_PASS);
    UT_EQUAL(stringx_compare(s1, "roUnd and sQuare", CASE_INSENS), RS_PASS);
    UT_EQUAL(stringx_compare(s1, "roUnd and sQuare", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_compare(s1, "xxx", CASE_SENS), RS_FAIL);
    UT_EQUAL(stringx_compare(s1, "xxx", CASE_INSENS), RS_FAIL);

    stringx_toupper(s1);
    UT_EQUAL(stringx_compare(s1, "ROUND AND SQUARE", CASE_SENS), RS_PASS);
    stringx_tolower(s1);
    UT_EQUAL(stringx_compare(s1, "round and square", CASE_SENS), RS_PASS);

    // Copy ops.
    stringx_t* s3 = stringx_copy(s1);
    UT_NOT_NULL(s3);
    UT_NOT_NULL(stringx_content(s3));
    UT_EQUAL(stringx_compare(s3, "round and square", CASE_SENS), RS_PASS);

    stringx_t* s4 = stringx_left(s2, 6);  //"The Mulberry Bush"
    UT_NOT_NULL(s4);
    UT_STR_EQUAL(stringx_content(s4), "The Mu");
    UT_STR_EQUAL(stringx_content(s2), "lberry Bush");

    // Append string.
    stringx_append(s2, s1);
    UT_STR_EQUAL(stringx_content(s2), "lberry Bushround and square");

    // Clean up.
    stringx_destroy(s1);
    stringx_destroy(s2);
    stringx_destroy(s3);
    stringx_destroy(s4);
    // Don't try to destroy the same object twice!
    //stringx_destroy(s4);

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STR_FANCY, "Test fancier stringx functions.")
{
    stringx_t* s1 = stringx_create("  round and square  ");
    stringx_t* s2 = stringx_create("");

    list_t* parts = stringx_split(s1, " ");
    UT_EQUAL(list_count(parts), 3);
    list_iterStart(parts);
    char* data;
    list_iterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "round");
    list_iterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "and");
    list_iterNext(parts, (void**)&data);
    UT_STR_EQUAL(data, "square");

    // Formatting.
    int i = 123;
    double d = 44.99;
    const char* s = "xyzzy oooo";

    UT_EQUAL(stringx_format(s2, 100, "i:%d I am a GOOD formatted string with d:%f s:%s x:%0X", i, d, s, i), RS_PASS);
    UT_EQUAL(stringx_compare(s2, "i:123 I am a GOOD formatted string with d:44.990000 s:xyzzy oooo x:7B", CASE_SENS), RS_PASS);

    // Clean up.
    stringx_destroy(s1);
    stringx_destroy(s2);
    list_destroy(parts);

    return 0;
}
