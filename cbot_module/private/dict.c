
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "list.h"
#include "dict.h"


/// @brief Definition of dictionary thing.

/// Prime number.
#ifndef DICT_NUM_BINS
#define DICT_NUM_BINS 101
#endif


//---------------- Private --------------------------//


/// One instance of a dictionary.
struct dict
{
    keyType_t kt;                   ///> The key type.
    list_t* bins[DICT_NUM_BINS];    ///> List data is kv_t.
};

/// Make hash from string and bin it.
/// @param s The string.
/// @return Hash value between 0 and DICT_NUM_BINS.
static unsigned int p_hashString(char* s);

/// Make hash from int and bin it.
/// @param i The int.
/// @return Hash value between 0 and DICT_NUM_BINS.
static unsigned int p_hashInt(int i);

//TODO check returns from list funcs = elsewhere.


//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
dict_t* dict_create(keyType_t kt)
{
    CREATE_INST(d, dict_t, BAD_PTR);

    // Initialize.
    d->kt = kt;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* l = list_create();
        VAL_PTR(l, BAD_PTR);
        d->bins[i] = l;
    }

    return d;
}

//--------------------------------------------------------//
int dict_destroy(dict_t* d)
{
    VAL_PTR(d, RS_ERR);

    int ret = RS_PASS;

    // Clean up user data.
    dict_clear(d);

    // Remove all bins.
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_destroy(d->bins[i]);
    }

    free(d);

    return ret;
}

//--------------------------------------------------------//
int dict_clear(dict_t* d)
{
    VAL_PTR(d, RS_ERR);

    int ret = RS_PASS;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        // Remove user data.
        kv_t* kv;
        list_iterStart(pl);
        while(RS_PASS == list_iterNext(pl, (void**)&kv))
        {
            VAL_PTR(kv, RS_ERR);
            if(d->kt == KEY_STRING && kv->key.ks != NULL)
            {
                free(kv->key.ks);
            }
        }

        list_clear(pl);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_count(dict_t* d)
{
    VAL_PTR(d, RS_ERR);

    int ret = 0;
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        ret += list_count(d->bins[i]);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_set(dict_t* d, kv_t* kv)
{
    VAL_PTR(d, RS_ERR);
    VAL_PTR(kv, RS_ERR);
    VAL_PTR(kv->key.ks, RS_ERR);

    int ret = RS_PASS;

    // If it is in a bin already, replace the value.
    unsigned int bin = d->kt == KEY_STRING ? p_hashString(kv->key.ks) : p_hashInt(kv->key.ki);
    list_t* pl = d->bins[bin]; // shorthand
    list_iterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(RS_PASS == list_iterNext(pl, (void**)&lkv) && !found)
    {
        VAL_PTR(lkv, RS_ERR);

        if(d->kt == KEY_STRING)
        {
            if(strcmp(lkv->key.ks, kv->key.ks) == 0)
            {
                lkv->value = kv->value;
                found = true;
            }
        }
        else // KEY_INT
        {
            if(lkv->key.ki == kv->key.ki)
            {
                lkv->value = kv->value;
                found = true;
            }
        }
    }

    // Not in a bin so add.
    if(!found)
    {
        list_append(pl, (void**)kv);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_get(dict_t* d, kv_t* kv)//, void** data)
{
    VAL_PTR(d, RS_ERR);
    VAL_PTR(kv, RS_ERR);

    int ret = RS_FAIL;
    kv->value = NULL;

    // Is it in the bin?
    unsigned int bin = d->kt == KEY_STRING ? p_hashString(kv->key.ks) : p_hashInt(kv->key.ki);
    list_t* pl = d->bins[bin]; // shorthand
    list_iterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(RS_PASS == list_iterNext(pl, (void**)&lkv) && !found)
    {
        VAL_PTR(lkv, RS_ERR);

        if(d->kt == KEY_STRING)
        {
            if(strcmp(lkv->key.ks, kv->key.ks) == 0)
            {
                ret = RS_PASS;
                kv->value = lkv->value;
                found = true;
            }
        }
        else // KEY_INT
        {
            if(lkv->key.ki == kv->key.ki)
            {
                ret = RS_PASS;
                kv->value = lkv->value;
                found = true;
            }
        }
    }

    return ret;
}

//--------------------------------------------------------//
list_t* dict_get_keys(dict_t* d)
{
    VAL_PTR(d, BAD_PTR);

    list_t* l = list_create();
    
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        list_iterStart(pl);

        kv_t* kv;

        while(RS_PASS == list_iterNext(pl, (void**)&kv))
        {
            VAL_PTR(kv, BAD_PTR);
            if(d->kt == KEY_STRING)
            {
                // Copy only.
                VAL_PTR(kv->key.ks, BAD_PTR);
                CREATE_STR(s, strlen(kv->key.ks), BAD_PTR);
                strcpy(s, kv->key.ks);
                list_append(l, s);
            }
            else // KEY_INT
            {
                CREATE_INST(pi, int, BAD_PTR);
                list_append(l, pi);
            }
            
        }
    }

    return l;
}

//--------------------------------------------------------//
int dict_dump(dict_t* d, FILE* fp)
{
    VAL_PTR(d, RS_ERR);
    VAL_PTR(fp, RS_ERR);

    int ret = RS_PASS;

    // Preamble.
    fprintf(fp, "type,bins,total\n");
    fprintf(fp, "%d,%d,%d\n\n", d->kt, DICT_NUM_BINS, dict_count(d));

    // Content.
    fprintf(fp, "bin,num,key0,key1,key2\n");

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        fprintf(fp, "%d,%d", i, list_count(pl));

        list_iterStart(pl);

        for(int k = 0; k < (int)fmin(list_count(pl), 3); k++)
        {
            fprintf(fp, ",");
            kv_t* kv;
            list_iterNext(pl, (void**)&kv);

            if(d->kt == KEY_STRING)
            {
                // Output and fix embedded commas, poorly.
                for(int ci = 0; ci < strlen(kv->key.ks); ci++)
                {
                    char c = kv->key.ks[ci];
                    fprintf(fp, "%c", c == ',' ? '#' : c);
                }
            }
            else // KEY_INT
            {
                fprintf(fp, "%d", kv->key.ki);
            }
        }

        fprintf(fp, "\n");
    }

    return ret;
}

//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
unsigned int p_hashString(char* s)
{
    // Lifted from http://www.cse.yorku.ca/~oz/hash.html.
    unsigned long long hash = 5381;
    int c;

    while ((c = *s++))
    {
        // djb2
        hash = ((hash << 5) + hash) + c; // aka: hash * 33 + c 
        // djb2 revised: hash = hash(i - 1) * 33 ^ str[i];
    }

    return (unsigned int)(hash % DICT_NUM_BINS);
}

//--------------------------------------------------------//
unsigned int p_hashInt(int i)
{
    return (unsigned int)(i % DICT_NUM_BINS);
}
