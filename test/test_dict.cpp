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
UT_SUITE(DICT_STR, "Test all dict functions using string key.")
{
    // Make a dict with string key.
    dict_t* mydict = dict_create(KEY_STRING);
    UT_NOT_NULL(mydict);

    // int dict_set(dict_t* d, void* key, void* data);

    // int dict_get(dict_t* d, void* key, void** data);

    // list_t* dict_get_keys(dict_t* d);

    // int dict_dump(dict_t* d, FILE* fp);



    // int dict_clear(dict_t* d);

    // int dict_destroy(dict_t* d);


    // Remove everything.
    dict_clear(mydict);
    UT_NOT_NULL(mydict);
    // UT_EQUAL(dict_count(mydict), 0);

    dict_destroy(mydict);



    // Remove everything.
    UT_EQUAL(dict_clear(mydict), RS_PASS);
    UT_NOT_NULL(mydict);
    // UT_EQUAL(list_count(mylist), 0);
    // UT_EQUAL(list_destroy(mylist), RS_PASS);

    // Bad container.
    dict_t* baddict = NULL;
    // UT_EQUAL(dict_push(baddict, &st1), RS_ERR);
    // UT_EQUAL(dict_append(baddict, &st3), RS_ERR);
    // UT_EQUAL(dict_push(baddict, &st4), RS_ERR);
    // UT_EQUAL(dict_count(baddict), RS_ERR);
    // UT_EQUAL(dict_next(baddict, (void**)&data), RS_ERR);
    // UT_EQUAL(dict_pop(baddict, (void**)&data), RS_ERR);
    UT_EQUAL(dict_clear(baddict), RS_ERR);
    UT_EQUAL(dict_destroy(baddict), RS_ERR);

}

/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_INT, "Test some dict functions using int key.")
{

}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DICT_DUMP, "Test the dump file creation.")
{
    // Make a dict with string key.
    dict_t* mydict = dict_create(KEY_STRING);
    UT_NOT_NULL(mydict);

    // Add some values.
    FILE* fp = fopen("hemingway_short.txt", "r");
    UT_NOT_NULL(fp);

    char buff[50];
    int buffind = 0;
    int i = 0;

    while(buffind >= 0)
    {
        char c = getc(fp);
        switch(c)
        {
            case ' ':
            case '\n':
            case EOF:
            // add entry
            if(buffind > 0)
            {
                buff[++buffind] = 0;
                CREATE_INST(st1, test_struct_t);
                st1->anumber = i;
                CREATE_STR(s, strlen(buff));
                strcpy(s, buff);
                i++;

                CREATE_INST(kv, kv_t);
                kv->key.ks = s;
                kv->value = st1;

                UT_EQUAL(dict_set(mydict, kv), RS_PASS);
                buffind = -1;
            }
            break;

            case '\r':
                // ignore
                break;

            default:
                // append
                buff[buffind++] = c;
                break;
        }
    }
    
    fclose(fp);

    // Dump it.
    fp = fopen("dict.csv", "w");
    UT_NOT_NULL(fp);
    UT_EQUAL(dict_dump(mydict, fp), RS_PASS);
    fclose(fp);
}
