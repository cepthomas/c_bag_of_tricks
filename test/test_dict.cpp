#include <cstdio>
#include <cstring>

#include "common.h"

#include "pnut.h"

extern "C"
{
#include "dict.h"
}

#define TEST_STR_LEN 16
// A data struct for testing. 
typedef struct
{
    int anumber;
    char astring[TEST_STR_LEN];
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

    test_struct_t* ts = NULL;
    test_struct_t* tsret = NULL;
    key_t key;

    // Good
    key.ks = "SOMETHING";
    UT_EQUAL(dict_get(mydict, key, (void**)&ts), RS_PASS);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 138);
    UT_STR_EQUAL(ts->astring, "Ajay_138");

    // Bad
    key.ks = "AAAAAA";
    UT_EQUAL(dict_get(mydict, key, (void**)&ts), RS_FAIL);

    // Replace one.
    // Create data payload.
    CREATE_INST(tsrep, test_struct_t, RS_ERR);
    tsrep->anumber = 9999;
    sprintf(tsrep->astring, "Ajay_%d", tsrep->anumber);
    // Create key/value.
 //   CREATE_STR(s3, 16, RS_ERR);
 //   strcpy(s3, "SOMETHING");
//    CREATE_INST(kv2, kv_t, RS_ERR);
    key.ks = "SOMETHING";
//    kv2->value = tsrep;
    dict_set(mydict, key, tsrep);
    // Size should not change.
    UT_EQUAL(dict_count(mydict), 184);
    // Content should have.
//    CREATE_STR(s4, 16, RS_ERR);
//    strcpy(s4, "SOMETHING");  
//    kv2->key.ks = s4;
    UT_EQUAL(dict_get(mydict, key, (void**)&tsret), RS_PASS);
    UT_NOT_NULL(tsret);
    //test_struct_t* tsr = (test_struct_t*)kv2->value;
    UT_EQUAL(tsret->anumber, 9999);
    UT_STR_EQUAL(tsret->astring, "Ajay_9999");

    // Get keys
    list_t* keys = dict_get_keys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_count(keys), 184);
    // look at some TODO...

    // Clean up everything.
    UT_EQUAL(dict_clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 0);
    UT_EQUAL(dict_destroy(mydict), RS_PASS);
    UT_EQUAL(list_destroy(keys), RS_PASS);

    // FREE(kv1);
    // FREE(kv2);
    // FREE(s1);
    // FREE(s2);
    // FREE(s3);
    // FREE(s4); //TODO Freeing invalid pointer 008D34D0.
//(214): Unfreed memory 00ED02B0.
//(33): Unfreed memory 00ED4390.
//(36): Unfreed memory 00ED3C18.
//(46): Unfreed memory 00ED3B98.
//(58): Unfreed memory 00ED3C38.
//(60): Unfreed memory 00ED4450.
//(67): Unfreed memory 00ED3C98.

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_INT, "Test some dict functions using int key.")
{
    // Make a dict with int key. create_int_dict() tests dict_create() and dict_set().
    dict_t* mydict = create_int_dict();
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 290);

    test_struct_t* ts = NULL;
 //   test_struct_t* tsret = NULL;
    key_t key;

//    CREATE_INST(kv, kv_t, RS_ERR);

    // good
    key.ki = 155;
    UT_EQUAL(dict_get(mydict, key, (void**)&ts), RS_PASS);
    UT_NOT_NULL(ts);
    UT_EQUAL(ts->anumber, 1155);
    UT_STR_EQUAL(ts->astring, "Boo_1155");
    // ng
    key.ki = 444;
    UT_EQUAL(dict_get(mydict, key, (void**)&ts), RS_FAIL);
    UT_NULL(ts);

    list_t* keys = dict_get_keys(mydict);
    UT_NOT_NULL(keys);
    UT_EQUAL(list_count(keys), 290);
    // look at some TODO...

    // Remove everything.
    UT_EQUAL(dict_clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    UT_EQUAL(dict_count(mydict), 0);
    UT_EQUAL(dict_destroy(mydict), RS_PASS);
    UT_EQUAL(list_destroy(keys), RS_PASS);

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

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_ERRORS, "Test some failure situations.")
{
    key_t key;
    void* value = NULL;

    // Bad container.
    dict_t* baddict = NULL;
    UT_EQUAL(dict_set(baddict, key, value), RS_ERR);
    UT_EQUAL(dict_dump(baddict, NULL), RS_ERR);
    UT_EQUAL(dict_get(baddict, key, &value), RS_ERR);
    UT_NULL(dict_get_keys(baddict));
    UT_EQUAL(dict_clear(baddict), RS_ERR);
    UT_EQUAL(dict_destroy(baddict), RS_ERR);
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
dict_t* create_str_dict(void)
{
    // Make a dict with string key.
    dict_t* d = dict_create(KEY_STRING);

    // Add some values.
    FILE* fp = fopen("hemingway_short.txt", "r");

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
                    CREATE_INST(st, test_struct_t, BAD_PTR); //TODO Unfreed memory 007DE998.
                    st->anumber = 100 + i++;
                    sprintf(st->astring, "Ajay_%d", st->anumber);

                    // Create key/value.
                    key_t key;
                    // CREATE_STR(skey, strlen(buff), BAD_PTR);
                    // strcpy(skey, buff);
                    // CREATE_INST(kv, kv_t, BAD_PTR);
                    key.ks = buff;
                    // kv->value = st;

                    dict_set(d, key, st);
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
        // Create data payload.
        CREATE_INST(st, test_struct_t, BAD_PTR);
        st->anumber = 1000 + k;
        sprintf(st->astring, "Boo_%d", st->anumber);

        // Create key/value.
        //CREATE_INST(kv, kv_t, BAD_PTR);
        key_t key;
        key.ki = k;
       // kv->value = st;

        dict_set(d, key, st);
    }

    return d;
}
