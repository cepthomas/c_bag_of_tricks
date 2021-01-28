#include <cstdio>
#include <cstring>

#include "common.h"

#include "pnut.h"

extern "C"
{
#include "list.h"
}

typedef struct
{
    int anumber;
    const char* astring;
} test_struct_t;



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LIST_ALL, "Test all list functions.")
{
    test_struct_t st1 { .anumber = 11, .astring = "Ajay1" };
    test_struct_t st2 { .anumber = 22, .astring = "Ajay2" };
    test_struct_t st3 { .anumber = 33, .astring = "Ajay3" };
    test_struct_t st4 { .anumber = 44, .astring = "Ajay4" };
    test_struct_t st5 { .anumber = 55, .astring = "Ajay5" };

    // Make a list.
    list_t* mylist = list_create();
    UT_NOT_NULL(mylist);

    // Add a node at the beginning.
    UT_EQUAL(list_push(mylist, &st1), RS_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_push(mylist, &st2), RS_PASS);

    // Add a node at the end.
    UT_EQUAL(list_append(mylist, &st3), RS_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_push(mylist, &st4), RS_PASS);

    UT_EQUAL(list_count(mylist), 4);

    // Iterate through list.
    test_struct_t* data;
    UT_EQUAL(list_iterStart(mylist), RS_PASS);
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
    UT_EQUAL(list_pop(mylist, (void**)&data), RS_PASS);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 33);
    UT_STR_EQUAL(data->astring, "Ajay3");
    // I own this now so clean up.
    free(data);

    // Add another.
    UT_EQUAL(list_push(mylist, &st5), RS_PASS);

    // Test pop.
    UT_EQUAL(list_pop(mylist, (void**)&data), RS_PASS);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 11);
    UT_STR_EQUAL(data->astring, "Ajay1");
    // I own this now so clean up.
    free(data);

    // Remove everything.
    UT_EQUAL(list_clear(mylist), RS_PASS);
    UT_NOT_NULL(mylist);
    UT_EQUAL(list_count(mylist), 0);
    UT_EQUAL(list_destroy(mylist), RS_PASS);

    // Bad container.
    list_t* badlist = NULL;
    UT_EQUAL(list_push(badlist, &st1), RS_ERR);
    UT_EQUAL(list_append(badlist, &st3), RS_ERR);
    UT_EQUAL(list_push(badlist, &st4), RS_ERR);
    UT_EQUAL(list_count(badlist), RS_ERR);
    UT_EQUAL(list_iterStart(badlist), RS_ERR);
    UT_EQUAL(list_iterNext(badlist, (void**)&data), RS_ERR);
    UT_EQUAL(list_pop(badlist, (void**)&data), RS_ERR);
    UT_EQUAL(list_clear(badlist), RS_ERR);
    UT_EQUAL(list_destroy(badlist), RS_ERR);
}
