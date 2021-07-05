#include <cstdio>
#include <cstring>

#include "common.h"

#include "pnut.h"

extern "C"
{
#include "list.h"
#include "dict.h"
}

typedef struct
{
    int anumber;
    const char* astring;
} test_struct_t;



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(INTERNAL_ALL, "Test internal.")
{
    test_struct_t st1 { .anumber = 11, .astring = "Ajay1" };
    test_struct_t st2 { .anumber = 22, .astring = "Ajay2" };
    test_struct_t st3 { .anumber = 33, .astring = "Ajay3" };
    test_struct_t st4 { .anumber = 44, .astring = "Ajay4" };
    test_struct_t st5 { .anumber = 55, .astring = "Ajay5" };

    // Make a list.
    list_t* mylist = list_create();
    UT_NOT_NULL(mylist);

    // Try to iterate an empty list.
    test_struct_t* data;
    UT_EQUAL(list_IterStart(mylist), RS_FAIL);
    UT_EQUAL(list_iterNext(mylist, (void**)&data), RS_FAIL);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, &st1), RS_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, &st2), RS_PASS);

    // Add a node at the end.
    UT_EQUAL(list_Append(mylist, &st3), RS_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, &st4), RS_PASS);

    UT_EQUAL(list_Count(mylist), 4);

    // Iterate through list.
    UT_EQUAL(list_IterStart(mylist), RS_PASS);
    int state = 0;

    while(RS_PASS == list_iterNext(mylist, (void**)&data))
    {
        UT_NOT_NULL(data);

        switch(state++)
        {
            case 0:
                UT_EQUAL(data->anumber, 44);
                UT_STR_EQUAL(data->astring, "Ajay4");
                break;

            case 1:
                UT_EQUAL(data->anumber, 22);
                UT_STR_EQUAL(data->astring, "Ajay2");
                break;

            case 2:
                UT_EQUAL(data->anumber, 11);
                UT_STR_EQUAL(data->astring, "Ajay1");
                break;

            case 3:
                UT_EQUAL(data->anumber, 33);
                UT_STR_EQUAL(data->astring, "Ajay3");
                break;
        }
    }

    // Try to take one more.
    UT_EQUAL(list_iterNext(mylist, (void**)&data), RS_FAIL);

    // Test pop.
    UT_EQUAL(list_Pop(mylist, (void**)&data), RS_PASS);
    UT_EQUAL(list_Count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 33);
    UT_STR_EQUAL(data->astring, "Ajay3");
    // I own this now so clean up.
    FREE(data);
    data = NULL;

    // Add another.
    UT_EQUAL(list_Push(mylist, &st5), RS_PASS);

    // Test pop.
    UT_EQUAL(list_Pop(mylist, (void**)&data), RS_PASS);
    UT_EQUAL(list_Count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 11);
    UT_STR_EQUAL(data->astring, "Ajay1");
    // I own this now so clean up.
    FREE(data);
    data = NULL;

    // Remove everything.
    UT_EQUAL(list_Clear(mylist), RS_PASS);
    UT_NOT_NULL(mylist);
    UT_EQUAL(list_Count(mylist), 0);
    UT_EQUAL(list_Destroy(mylist), RS_PASS);

    // Bad container.
    list_t* badlist = NULL;
    UT_EQUAL(list_Push(badlist, &st1), RS_ERR);
    UT_EQUAL(list_Append(badlist, &st3), RS_ERR);
    UT_EQUAL(list_Push(badlist, &st4), RS_ERR);
    UT_EQUAL(list_Count(badlist), RS_ERR);
    UT_EQUAL(list_IterStart(badlist), RS_ERR);
    UT_EQUAL(list_iterNext(badlist, (void**)&data), RS_ERR);
    UT_EQUAL(list_Pop(badlist, (void**)&data), RS_ERR);
    UT_EQUAL(list_Clear(badlist), RS_ERR);
    UT_EQUAL(list_Destroy(badlist), RS_ERR);
}
