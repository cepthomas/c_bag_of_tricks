#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "list.h"
}

typedef struct test_struct
{
    int anumber;
    const char* astring;
} test_struct_t;


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LIST_1, "Test all list functions.")
{
    int i;

    test_struct_t st1 = { 11, "Ajay1" };
    test_struct_t st2 = { 22, "Ajay2" };
    test_struct_t st3 = { 33, "Ajay3" };
    test_struct_t st4 = { 44, "Ajay4" };
    test_struct_t st5 = { 55, "Ajay5" };

    // Make a list.
    list_t* mylist = list_create();

    // Add a node at the beginning.
    listData_t d1 = {&st1};
    list_push(mylist, d1);

    // Add a node at the beginning.
    listData_t d2 = {&st2};
    list_push(mylist, d2);

    // Add a node at the end.
    listData_t d3 = {&st3};
    list_append(mylist, d3);

    // Add a node at the beginning.
    listData_t d4 = {&st4};
    list_push(mylist, d4);

    UT_EQUAL(list_count(mylist), 4);

    // Iterate through list.
    i = 0;
    list_start(mylist);

    listData_t data;
    while(list_next(mylist, &data))
    {
        auto* ts = static_cast<test_struct_t*>(data.p);
        UT_NOT_NULL(ts);

        switch(i)
        {
            case 0:
                UT_EQUAL(ts->anumber, 44);
                UT_STR_EQUAL(ts->astring, "Ajay4");
                break;

            case 1:
                UT_EQUAL(ts->anumber, 22);
                UT_STR_EQUAL(ts->astring, "Ajay2");
                break;

            case 2:
                UT_EQUAL(ts->anumber, 11);
                UT_STR_EQUAL(ts->astring, "Ajay1");
                break;

            case 3:
                UT_EQUAL(ts->anumber, 33);
                UT_STR_EQUAL(ts->astring, "Ajay3");
                break;
        }
        i++;
    }

    // Test pop.
    bool ok = list_pop(mylist, &data);
    UT_TRUE(ok);

    auto* ts = static_cast<test_struct_t*>(data.p);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 33);
    UT_STR_EQUAL(ts->astring, "Ajay3");

    // Add another.
    listData_t d5 = {&st5};
    list_push(mylist, d5);

    // Test pop.
    ok = list_pop(mylist, &data);
    UT_TRUE(ok);
    ts = static_cast<test_struct_t*>(data.p);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 11);
    UT_STR_EQUAL(ts->astring, "Ajay1");

    // Remove everything.
    list_clear(mylist);
    UT_NOT_NULL(mylist);
    UT_EQUAL(list_count(mylist), 0);
    free(mylist);
}
