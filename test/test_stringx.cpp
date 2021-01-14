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

    stringx_t* s2 = stringx_create(NULL);
    UT_NOT_NULL(stringx_content(s2));
    UT_EQUAL(stringx_len(s2), 0);
    UT_STR_EQUAL(stringx_content(s2), "");

    stringx_set(s2, "The Mulberry Bush");
    UT_NOT_NULL(stringx_content(s2));
    UT_EQUAL(stringx_len(s2), 17);
    UT_STR_EQUAL(stringx_content(s2), "The Mulberry Bush");

    stringx_trim(s1);
    UT_STR_EQUAL(stringx_content(s1), "round and square");

    UT_TRUE(stringx_starts(s1, "round", CASE_SENS));
    UT_FALSE(stringx_starts(s1, "Round", CASE_SENS));
    UT_TRUE(stringx_starts(s1, "Round", CASE_INSENS));
    UT_FALSE(stringx_starts(s1, "xxx", CASE_SENS));
    UT_FALSE(stringx_starts(s1, "xxx", CASE_INSENS));

    UT_FALSE(stringx_ends(s2, "bush", CASE_SENS));
    UT_TRUE(stringx_ends(s2, "Bush", CASE_SENS));
    UT_TRUE(stringx_ends(s2, "bush", CASE_INSENS));
    UT_FALSE(stringx_ends(s2, "xxx", CASE_SENS));
    UT_FALSE(stringx_ends(s2, "xxx", CASE_INSENS));

//    UT_TRUE(stringx_contains(s1, "and squ", CASE_SENS));
//    UT_FALSE(stringx_contains(s1, "anD squ", CASE_SENS));
//    UT_TRUE(stringx_contains(s1, "anD squ", CASE_INSENS));
//    UT_FALSE(stringx_contains(s1, "xxx", CASE_SENS));
//    UT_FALSE(stringx_contains(s1, "xxx", CASE_INSENS));

    UT_TRUE(stringx_compare(s1, "round and square", CASE_SENS));
    UT_TRUE(stringx_compare(s1, "roUnd and sQuare", CASE_INSENS));
    UT_FALSE(stringx_compare(s1, "roUnd and sQuare", CASE_SENS));
    UT_FALSE(stringx_compare(s1, "xxx", CASE_SENS));
    UT_FALSE(stringx_compare(s1, "xxx", CASE_INSENS));

    stringx_upper(s1);
    UT_TRUE(stringx_compare(s1, "ROUND AND SQUARE", CASE_SENS));
    stringx_lower(s1);
    UT_TRUE(stringx_compare(s1, "round and square", CASE_SENS));

    // Copy ops.
    stringx_t* s3 = stringx_copy(s1);
    UT_NOT_NULL(s3);
    UT_NOT_NULL(stringx_content(s3));
    UT_TRUE(stringx_compare(s3, "round and square", CASE_SENS));

    stringx_t* s4 = stringx_left(s2, 6);  //"The Mulberry Bush"
    UT_NOT_NULL(s4);
    UT_STR_EQUAL(stringx_content(s4), "The Mu");
    UT_STR_EQUAL(stringx_content(s2), "lberry Bush");

    // Append char.
    stringx_append(s2, ' ');
    stringx_append(s2, 'x');
    stringx_append(s2, 'y');
    stringx_append(s2, 'z');
    UT_STR_EQUAL(stringx_content(s2), "lberry Bush xyz");

    // Clean up.
    stringx_destroy(s1);
    stringx_destroy(s2);
    stringx_destroy(s3);
    stringx_destroy(s4);
    // Don't try to destroy the same object twice!
    //stringx_destroy(s4);
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(STR_FANCY, "Test fancier stringx functions.")
{
    stringx_t* s1 = stringx_create("  round and square  ");
    stringx_t* s2 = stringx_create(NULL);

    list_t* parts = stringx_split(s1, " ");
    UT_EQUAL(list_count(parts), 3);
    list_start(parts);
    void* data;
    list_next(parts, &data);
    UT_STR_EQUAL(static_cast<char*>(data), "round");
    list_next(parts, &data);
    UT_STR_EQUAL(static_cast<char*>(data), "and");
    list_next(parts, &data);
    UT_STR_EQUAL(static_cast<char*>(data), "square");

    // Formatting.
    int i = 123;
    double d = 44.99;
    const char* s = "xyzzy oooo";

    UT_TRUE(stringx_format(s2, 100, "i:%d I am a GOOD formatted string with d:%f s:%s x:%0X", i, d, s, i));
    UT_TRUE(stringx_compare(s2, "i:123 I am a GOOD formatted string with d:44.990000 s:xyzzy oooo x:7B", CASE_SENS));

    // Clean up.
    stringx_destroy(s1);
    stringx_destroy(s2);
    list_destroy(parts);
}
