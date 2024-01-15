#include <cstdio>
#include <cstring>
#include <errno.h>

#include "pnut.h"

extern "C"
{
#include "common.h"
#include "dict.h"
}

static const int TEST_STR_LEN = 16;

// A data struct for testing.
typedef struct
{
    int anumber;
    char astring[TEST_STR_LEN+1];
} test_struct_t;

// Helpers.
dict_t* create_str_dict(void);
dict_t* create_int_dict(void);


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_STR, "Test all dict functions using string key.")
{
    return 0;
    // Make a dict with string key. create_str_dict() tests dict_create() and dict_Set().
    dict_t* mydict = create_str_dict();
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_Count(mydict), 184);

    test_struct_t* ts = NULL;
    test_struct_t* tsret = NULL;
    key_t key;

    FILE* fpd = fopen("dump.txt", "w");
    dict_Dump(mydict, fpd);
    fclose(fpd);

    // Good
    key.ks = "SOMETHING";
    UT_EQUAL(dict_Get(mydict, key, (void**)&ts), RS_PASS);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 138);
    UT_STR_EQUAL(ts->astring, "Ajay_138");

    // Bad
    key.ks = "AAAAAA";
    UT_EQUAL(dict_Get(mydict, key, (void**)&ts), RS_FAIL);

    // Replace one.
    // Create data payload.
    CREATE_INST(tsrep, test_struct_t);
    tsrep->anumber = 9999;
    snprintf(tsrep->astring, TEST_STR_LEN, "Ajay_%d", tsrep->anumber);
    // Create key/value.
    key.ks = "SOMETHING";
    dict_Set(mydict, key, tsrep);
    // Size should not change.
    UT_EQUAL(dict_Count(mydict), 184);
    // Content should have.
    UT_EQUAL(dict_Get(mydict, key, (void**)&tsret), RS_PASS);
    UT_NOT_NULL(tsret);
    UT_EQUAL(tsret->anumber, 9999);
    UT_STR_EQUAL(tsret->astring, "Ajay_9999");

    // Get keys
    list_t* keys = dict_GetKeys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_Count(keys), 184);
    // Should look at some ...

    // Clean up everything.
    UT_EQUAL(dict_Clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_Count(mydict), 0);
    UT_EQUAL(dict_Destroy(mydict), RS_PASS);
    UT_EQUAL(list_Destroy(keys), RS_PASS);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_INT, "Test some dict functions using int key.")
{
    // Make a dict with int key. create_int_dict() tests dict_create() and dict_Set().
    dict_t* mydict = create_int_dict();
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_Count(mydict), 290);

    test_struct_t* ts = NULL;
    key_t key;

    // good
    key.ki = 155;
    UT_EQUAL(dict_Get(mydict, key, (void**)&ts), RS_PASS);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 1155);
    UT_STR_EQUAL(ts->astring, "Boo_1155");
    // ng
    ts = NULL;
    key.ki = 444;
    UT_EQUAL(dict_Get(mydict, key, (void**)&ts), RS_FAIL);
    UT_NULL(ts);

    list_t* keys = dict_GetKeys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_Count(keys), 290);
    // Should look at some ...

    // Remove everything.
    UT_EQUAL(dict_Clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_Count(mydict), 0);
    UT_EQUAL(dict_Destroy(mydict), RS_PASS);
    UT_EQUAL(list_Destroy(keys), RS_PASS);

//    FREE(kv);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_DUMP, "Test the dump file creation.")
{
    // string flavor
    dict_t* mydict = create_str_dict();
    UT_NOT_NULL(mydict);

    // Dump it.
    FILE* fp = fopen("dict_str_out.csv", "w");
    UT_NOT_NULL(fp);
    UT_EQUAL(dict_Dump(mydict, fp), RS_PASS);
    fclose(fp);
    dict_Destroy(mydict);

    // int flavor
    mydict = create_int_dict();
    UT_NOT_NULL(mydict);

    // Dump it.
    fp = fopen("dict_int_out.csv", "w");
    UT_NOT_NULL(fp);
    UT_EQUAL(dict_Dump(mydict, fp), RS_PASS);
    fclose(fp);
    dict_Destroy(mydict);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_ERRORS, "Test some failure situations.")
{
    key_t key;
    void* value = NULL;

    // Bad container.
    dict_t* baddict = NULL;
    UT_EQUAL(dict_Set(baddict, key, value), RS_ERR);
    UT_EQUAL(dict_Dump(baddict, NULL), RS_ERR);
    UT_EQUAL(dict_Get(baddict, key, &value), RS_ERR);
    UT_NULL(dict_GetKeys(baddict));
    UT_EQUAL(dict_Clear(baddict), RS_ERR);
    UT_EQUAL(dict_Destroy(baddict), RS_ERR);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
dict_t* create_str_dict(void)
{
    // Make a dict with string key.
    dict_t* d = dict_Create(KEY_STRING);

    // Add some values.
    FILE* fp = fopen("..\\test\\files\\hemingway_short.txt", "r");

    char buff[64]; // I just know this.
    int buffind = 0;
    int i = 0;
    bool done = false;

    while(!done)
    {
        char c = getc(fp);
        switch(c)
        {
            case EOF:
                done = true;
                // fall through to clean up
            case '\n':
                // add entry if exists
                if(buffind > 0)
                {
                    buff[buffind] = 0; // terminate

                    // Create data payload.
                    CREATE_INST(st, test_struct_t);
                    st->anumber = 100 + i++;
                    snprintf(st->astring, TEST_STR_LEN, "Ajay_%d", st->anumber);

                    // Create key/value.
                    key_t key;
                    key.ks = buff;

                    dict_Set(d, key, st);
                    buffind = 0;
                }
                break;

            case '\r':
            case '\b':
            case '\t':
            case '\v':
            case '\f':
                // ignore
                break;

            default:
                // append
                buff[buffind++] = c;
                break;
        }
    }
    
    fclose(fp);

    return d;
}

//////////////////////////////////////////////////////////////////////////
dict_t* create_int_dict(void)
{
    // Make a dict with int key.
    dict_t* d = dict_Create(KEY_INT);

    // Add some values.
    for(int k = 0; k < 290; k++)
    {
        // Create data payload.
        CREATE_INST(st, test_struct_t);
        st->anumber = 1000 + k;
        snprintf(st->astring, TEST_STR_LEN, "Boo_%d", st->anumber);

        // Create key/value.
        key_t key;
        key.ki = k;

        dict_Set(d, key, st);
    }

    return d;
}
