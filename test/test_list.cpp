#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "diagnostics.h"
#include "status.h"
#include "list.h"
}

static const int TEST_STR_LEN = 16;

// A data struct for testing. 
typedef struct
{
    int anumber;
    char astring[TEST_STR_LEN];
} test_struct_t;



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(LIST_ALL, "Test all list functions.")
{
    const int NUM_TS = 5;
    test_struct_t* ts[NUM_TS];

    for(int i = 0; i < NUM_TS; i++)
    {
        CREATE_INST(st, test_struct_t);
        st->anumber = 11 * (i + 1);
        snprintf(st->astring, TEST_STR_LEN-1, "Ajay%d", st->anumber);
        ts[i] = st;
    }

    // Make a list.
    list_t* mylist = list_Create();
    UT_NOT_NULL(mylist);

    // Try to iterate an empty list.
    test_struct_t* data;
    UT_EQUAL(list_IterStart(mylist), EINVALIDINDEX);
    UT_EQUAL(list_IterNext(mylist, (void**)&data), EINVALIDINDEX);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, ts[0]), ENOERR);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, ts[1]), ENOERR);

    // Add a node at the end.
    UT_EQUAL(list_Append(mylist, ts[2]), ENOERR);

    // Add a node at the beginning.
    UT_EQUAL(list_Push(mylist, ts[3]), ENOERR);

    int cnt = list_Count(mylist);
    UT_EQUAL(cnt, 4);

    // Iterate through list.
    UT_EQUAL(list_IterStart(mylist), ENOERR);
    int state = 0;

    while(list_IterNext(mylist, (void**)&data) == 0)
    {
        UT_NOT_NULL(data);

        switch(state++)
        {
            case 0:
                UT_EQUAL(data->anumber, 44);
                UT_STR_EQUAL(data->astring, "Ajay44");
                break;

            case 1:
                UT_EQUAL(data->anumber, 22);
                UT_STR_EQUAL(data->astring, "Ajay22");
                break;

            case 2:
                UT_EQUAL(data->anumber, 11);
                UT_STR_EQUAL(data->astring, "Ajay11");
                break;

            case 3:
                UT_EQUAL(data->anumber, 33);
                UT_STR_EQUAL(data->astring, "Ajay33");
                break;
        }
    }

    // Try to take one more.
    UT_EQUAL(list_IterNext(mylist, (void**)&data), EINVALIDINDEX);

    // Test pop.
    UT_EQUAL(list_Pop(mylist, (void**)&data), ENOERR);
    cnt = list_Count(mylist);
    UT_EQUAL(cnt, 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 33);
    UT_STR_EQUAL(data->astring, "Ajay33");
    // I own this now so clean up.
    FREE(data);
    data = NULL;

    // Add another.
    UT_EQUAL(list_Push(mylist, ts[4]), ENOERR);

    // Test pop.
    UT_EQUAL(list_Pop(mylist, (void**)&data), ENOERR);
    cnt = list_Count(mylist);
    UT_EQUAL(cnt, 3);
    UT_NOT_NULL(data);
    UT_EQUAL(data->anumber, 11);
    UT_STR_EQUAL(data->astring, "Ajay11");
    // I own this now so clean up.
    FREE(data);
    data = NULL;

    // Remove everything.
    UT_EQUAL(list_Clear(mylist), ENOERR);
    UT_NOT_NULL(mylist);
    cnt = list_Count(mylist);
    UT_EQUAL(cnt, 0);
    UT_EQUAL(list_Destroy(mylist), ENOERR);

    // Bad container.
    list_t* badlist = NULL;
    UT_EQUAL(list_Push(badlist, ts[0]), EARGNULL);
    UT_EQUAL(list_Append(badlist, ts[0]), EARGNULL);
    UT_EQUAL(list_Push(badlist, ts[0]), EARGNULL);
    UT_EQUAL(list_Count(badlist), -EARGNULL);
    UT_EQUAL(list_IterStart(badlist), EARGNULL);
    UT_EQUAL(list_IterNext(badlist, (void**)&data), EARGNULL);
    UT_EQUAL(list_Pop(badlist, (void**)&data), EARGNULL);
    UT_EQUAL(list_Clear(badlist), EARGNULL);
    UT_EQUAL(list_Destroy(badlist), EARGNULL);

    return 0;
}
