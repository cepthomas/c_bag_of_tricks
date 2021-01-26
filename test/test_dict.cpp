#include <cstdio>
#include <cstring>

#include "common.h"

#include "pnut.h"

extern "C"
{
#include "dict.h"
}

typedef struct
{
    int anumber;
    const char* astring;
} test_struct_t;



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_ALL, "Test all dict functions.")
{
    // int i;

    // test_struct_t st1 { .anumber = 11, .astring = "Ajay1" };
    // test_struct_t st2 { .anumber = 22, .astring = "Ajay2" };
    // test_struct_t st3 { .anumber = 33, .astring = "Ajay3" };
    // test_struct_t st4 { .anumber = 44, .astring = "Ajay4" };
    // test_struct_t st5 { .anumber = 55, .astring = "Ajay5" };

    // Make a dict.
    dict_t* mydict = dict_create(KEY_STRING);


    // Remove everything.
    dict_clear(mydict);
    UT_NOT_NULL(mydict);
//    UT_EQUAL(dict_count(mydict), 0);

    dict_destroy(mydict);
}
