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
    UT_EQUAL(list_push(mylist, &st1), RET_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_push(mylist, &st2), RET_PASS);

    // Add a node at the end.
    UT_EQUAL(list_append(mylist, &st3), RET_PASS);

    // Add a node at the beginning.
    UT_EQUAL(list_push(mylist, &st4), RET_PASS);

    UT_EQUAL(list_count(mylist), 4);

    // Iterate through list.
    test_struct_t* data;
    UT_EQUAL(list_start(mylist), RET_PASS);
    int state = 0;

    while(RET_PASS == list_next(mylist, (void**)&data))
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
    UT_EQUAL(list_next(mylist, (void**)&data), RET_FAIL);

    // Test pop.
    UT_EQUAL(list_pop(mylist, (void**)&data), RET_PASS);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 33);
    UT_STR_EQUAL(data->astring, "Ajay3");
    // I own this now so clean up.
    free(data);

    // Add another.
    UT_EQUAL(list_push(mylist, &st5), RET_PASS);

    // Test pop.
    UT_EQUAL(list_pop(mylist, (void**)&data), RET_PASS);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 11);
    UT_STR_EQUAL(data->astring, "Ajay1");
    // I own this now so clean up.
    free(data);

    // Remove everything.
    UT_EQUAL(list_clear(mylist), RET_PASS);
    UT_NOT_NULL(mylist);
    UT_EQUAL(list_count(mylist), 0);
    UT_EQUAL(list_destroy(mylist), RET_PASS);

    // Bad container.
    list_t* badlist = NULL;
    UT_EQUAL(list_push(badlist, &st1), RET_ERR);
    UT_EQUAL(list_append(badlist, &st3), RET_ERR);
    UT_EQUAL(list_push(badlist, &st4), RET_ERR);
    UT_EQUAL(list_count(badlist), RET_ERR);
    UT_EQUAL(list_next(badlist, (void**)&data), RET_ERR);
    UT_EQUAL(list_pop(badlist, (void**)&data), RET_ERR);
    UT_EQUAL(list_clear(badlist), RET_ERR);
    UT_EQUAL(list_destroy(badlist), RET_ERR);
}
