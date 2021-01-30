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

// Helpers.
dict_t* create_str_dict(void);
dict_t* create_int_dict(void);


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_STR, "Test all dict functions using string key.")
{
    // Make a dict with string key. create_str_dict() tests dict_create() and dict_set().
    dict_t* mydict = create_str_dict();
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 184);

    kv_t* kv = (kv_t*)calloc(1, sizeof(kv_t));

    // look at some
    char* s = (char*)calloc(10, sizeof(char));

    // good
    strcpy(s, "SOMETHING");  
    kv->key.ks = s;
    UT_EQUAL(dict_get(mydict, kv), RS_PASS);
    UT_NOT_NULL(kv->value);
    test_struct_t* ts = (test_struct_t*)kv->value;
    UT_EQUAL(ts->anumber, 138);
    UT_STR_EQUAL(ts->astring, "str138");
    // ng
    strcpy(s, "AAAAAA"); 
    kv->key.ks = s;
    UT_EQUAL(dict_get(mydict, kv), RS_FAIL);
    UT_NULL(kv->value);

    list_t* keys = dict_get_keys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_count(keys), 184);
    // look at some TODO...

    // Remove everything.
    UT_EQUAL(dict_clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 0);
    UT_EQUAL(dict_destroy(mydict), RS_PASS);

    // Bad container.
    dict_t* baddict = NULL;
    UT_EQUAL(dict_set(baddict, kv), RS_ERR);
    UT_EQUAL(dict_dump(baddict, NULL), RS_ERR);
    UT_EQUAL(dict_get(baddict, kv), RS_ERR);
    UT_NULL(dict_get_keys(baddict));
    UT_EQUAL(dict_clear(baddict), RS_ERR);
    UT_EQUAL(dict_destroy(baddict), RS_ERR);
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_INT, "Test some dict functions using int key.")
{
    // Make a dict with int key. create_int_dict() tests dict_create() and dict_set().
    dict_t* mydict = create_int_dict();
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 289);

    kv_t* kv = (kv_t*)calloc(1, sizeof(kv_t));

    // look at some
    // good
    kv->key.ki = 155;
    UT_EQUAL(dict_get(mydict, kv), RS_PASS);
    UT_NOT_NULL(kv->value);
    test_struct_t* ts = (test_struct_t*)kv->value;
    UT_EQUAL(ts->anumber, 1155);
    UT_STR_EQUAL(ts->astring, "str155");
    // ng
    kv->key.ki = 444;
    UT_EQUAL(dict_get(mydict, kv), RS_FAIL);
    UT_NULL(kv->value);

    list_t* keys = dict_get_keys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_count(keys), 289);
    // look at some TODO...

    // Remove everything.
    UT_EQUAL(dict_clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 0);
    UT_EQUAL(dict_destroy(mydict), RS_PASS);
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_DUMP, "Test the dump file creation.")
{
    // string flavor
    dict_t* mydict = create_str_dict();
    UT_NOT_NULL(mydict);

    // Dump it.
    FILE* fp = fopen("dict_str.csv", "w");
    UT_NOT_NULL(fp);
    UT_EQUAL(dict_dump(mydict, fp), RS_PASS);
    fclose(fp);
    dict_destroy(mydict);

    // int flavor
    mydict = create_int_dict();
    UT_NOT_NULL(mydict);

    // Dump it.
    fp = fopen("dict_int.csv", "w");
    UT_NOT_NULL(fp);
    UT_EQUAL(dict_dump(mydict, fp), RS_PASS);
    fclose(fp);
    dict_destroy(mydict);

    // TODO need some actual test.
}

/////////////////////////////////////////////////////////////////////////////
dict_t* create_str_dict(void)
{
    // Make a dict with string key.
    dict_t* d = dict_create(KEY_STRING);

    // Add some values.
    FILE* fp = fopen("hemingway_short.txt", "r");

    char buff[50];
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
                    buff[buffind] = 0;
                    CREATE_INST(st1, test_struct_t, NULL);
                    CREATE_STR(sv, 16, NULL);
                    st1->anumber = 100 + i;
                    sprintf(sv, "str%d", st1->anumber);
                    st1->astring = sv;
                    CREATE_STR(sk, strlen(buff), NULL);
                    strcpy(sk, buff);
                    i++;

                    CREATE_INST(kv, kv_t, NULL);
                    kv->key.ks = sk;
                    kv->value = st1;

                    dict_set(d, kv);
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

/////////////////////////////////////////////////////////////////////////////
dict_t* create_int_dict(void)
{
    // Make a dict with int key.
    dict_t* d = dict_create(KEY_INT);

    // Add some values.
    for(int k = 0; k < 290; k++)
    {
        test_struct_t* st1 = (test_struct_t*)calloc(1, sizeof(test_struct_t));
        st1->anumber = 1000 + k;
        char* s = (char*)calloc(10, sizeof(char));
        sprintf(s, "str%d", k);
        st1->astring = s;
        kv_t* kv = (kv_t*)calloc(1, sizeof(kv_t));
        kv->key.ki = k;
        kv->value = st1;
        dict_set(d, kv);
    }

    return d;
}
