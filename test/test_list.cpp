#include <cstdio>
#include <cstring>

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
    int i;

    test_struct_t* st1 = static_cast<test_struct_t*>(malloc(sizeof(test_struct_t)));
    st1->anumber = 11;
    st1->astring = "Ajay1";

    test_struct_t* st2 = static_cast<test_struct_t*>(malloc(sizeof(test_struct_t)));
    st2->anumber = 22;
    st2->astring = "Ajay2";

    test_struct_t* st3 = static_cast<test_struct_t*>(malloc(sizeof(test_struct_t)));
    st3->anumber = 33;
    st3->astring = "Ajay3";

    test_struct_t* st4 = static_cast<test_struct_t*>(malloc(sizeof(test_struct_t)));
    st4->anumber = 44;
    st4->astring = "Ajay4";

    test_struct_t* st5 = static_cast<test_struct_t*>(malloc(sizeof(test_struct_t)));
    st5->anumber = 55;
    st5->astring = "Ajay5";

    // Make a list.
    list_t* mylist = list_create();

    // Add a node at the beginning.
    list_push(mylist, st1);

    // Add a node at the beginning.
    list_push(mylist, st2);

    // Add a node at the end.
    list_append(mylist, st3);

    // Add a node at the beginning.
    list_push(mylist, st4);

    UT_EQUAL(list_count(mylist), 4);

    // Iterate through list.
    i = 0;
    void* data;
    list_start(mylist);
    while(list_next(mylist, &data))
    {
        auto* ts = static_cast<test_struct_t*>(data);
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

    auto* ts = static_cast<test_struct_t*>(data);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 33);
    UT_STR_EQUAL(ts->astring, "Ajay3");
    // I own this now so clean up.
    free(data);

    // Add another.
    list_push(mylist, st5);

    // Test pop.
    ok = list_pop(mylist, &data);
    UT_TRUE(ok);
    ts = static_cast<test_struct_t*>(data);
    UT_EQUAL(list_count(mylist), 3);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 11);
    UT_STR_EQUAL(ts->astring, "Ajay1");
    // I own this now so clean up.
    free(data);

    // Remove everything.
    list_clear(mylist);
    UT_NOT_NULL(mylist);
    UT_EQUAL(list_count(mylist), 0);

    list_destroy(mylist);
}
