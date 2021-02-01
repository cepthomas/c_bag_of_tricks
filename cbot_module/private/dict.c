
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "list.h"
#include "dict.h"


/// @brief Definition of dictionary thing.

/// Size should be a prime number. TODO Would be nice to set from dict_create() arg.
#define DICT_NUM_BINS 101


//---------------- Private --------------------------//

/// Key-value pair.
typedef struct kv
{
    char* skey;     ///> The key.
    int ikey;       ///> The key.
    void* value;    ///> Client specific data. Client must cast.
} kv_t;


/// One instance of a dictionary.
struct dict
{
    keyType_t kt;                   ///> The key type.
    list_t* bins[DICT_NUM_BINS];    ///> List data is kv_t.
};

/// Make hash from string and bin it.
/// @param s The string.
/// @return Hash value between 0 and DICT_NUM_BINS.
static unsigned int p_hashString(const char* s);

/// Make hash from int and bin it.
/// @param i The int.
/// @return Hash value between 0 and DICT_NUM_BINS.
static unsigned int p_hashInt(int i);

/// Convert client to internal format.
/// @param kt Type.
/// @param k Key itself.
/// @return New pointer to thing.
static kv_t* p_convertKey(keyType_t kt, key_t k);

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

    FREE(d);

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
        VAL_PTR(pl, RS_ERR);

        // Remove custom user data.
        kv_t* kv;
        list_iterStart(pl);

        while(RS_PASS == list_iterNext(pl, (void**)&kv))
        {
            VAL_PTR(kv, RS_ERR);
            if(d->kt == KEY_STRING && kv->skey != NULL)
            {
                FREE(kv->skey);
                kv->skey = NULL;
            }

            if(kv->value != NULL)
            {
                FREE(kv->value);
                kv->value = NULL;
            }

            // kv gets freed in list_clear()
        }

        ret = list_clear(pl);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_count(dict_t* d)
{
    VAL_PTR(d, RS_ERR);

    int ret = 0;
    for(int i = 0; i < DICT_NUM_BINS && ret >= 0; i++)
    {
        int cnt = list_count(d->bins[i]);
        if(cnt >= 0)
        {
            ret += cnt;
        }
        else
        {
            ret = cnt; // ng
        }
    }

    return ret;
}

//--------------------------------------------------------//
int dict_set(dict_t* d, key_t k, void* v)
{
    VAL_PTR(d, RS_ERR);
    VAL_PTR(v, RS_ERR);

    int ret = RS_PASS;


    // If it is in a bin already, replace the value.
    unsigned int bin = d->kt == KEY_STRING ? p_hashString(k.ks) : p_hashInt(k.ki);
    list_t* pl = d->bins[bin]; // shorthand
    VAL_PTR(pl, RS_ERR);

    list_iterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(RS_PASS == list_iterNext(pl, (void**)&lkv) && !found)
    {
        VAL_PTR(lkv, RS_ERR);

        if(d->kt == KEY_STRING)
        {
            found = (strcmp(lkv->skey, k.ks) == 0);
        }
        else // KEY_INT
        {
            found = (lkv->ikey == k.ki);
        }

        if(found)
        {
            // Need to FREE the original data then copy from the new..
            if(lkv->value != NULL)
            {
                FREE(lkv->value);
            }
            lkv->value = v;
        }
    }

    // Not in a bin so add.
    if(!found)
    {
        // Pack into our internal format.
        kv_t* kv = p_convertKey(d->kt, k);
        kv->value = v;

        list_append(pl, (void*)kv);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_get(dict_t* d, key_t k, void** v)
{
    VAL_PTR(d, RS_ERR);

    int ret = RS_FAIL;

    // Is it in the bin?
    unsigned int bin = d->kt == KEY_STRING ? p_hashString(k.ks) : p_hashInt(k.ki);
    list_t* pl = d->bins[bin]; // shorthand

    list_iterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(RS_PASS == list_iterNext(pl, (void**)&lkv) && !found)
    {
        VAL_PTR(lkv, RS_ERR);

        if(d->kt == KEY_STRING)
        {
            if(strcmp(lkv->skey, k.ks) == 0)
            {
                ret = RS_PASS;
                *v = lkv->value;
                found = true;
            }
        }
        else // KEY_INT
        {
            if(lkv->ikey == k.ki)
            {
                ret = RS_PASS;
                *v = lkv->value;
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
    VAL_PTR(l, BAD_PTR);
    
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand
        VAL_PTR(pl, BAD_PTR);

        list_iterStart(pl);

        kv_t* kv;

        while(RS_PASS == list_iterNext(pl, (void**)&kv))
        {
            VAL_PTR(kv, BAD_PTR);
            if(d->kt == KEY_STRING)
            {
                // Copy only.
                VAL_PTR(kv->skey, BAD_PTR);
                CREATE_STR(s, strlen(kv->skey), BAD_PTR);
                strcpy(s, kv->skey);
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
        VAL_PTR(pl, RS_ERR);

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
                for(int ci = 0; ci < strlen(kv->skey); ci++)
                {
                    char c = kv->skey[ci];
                    fprintf(fp, "%c", c == ',' ? '#' : c);
                }
            }
            else // KEY_INT
            {
                fprintf(fp, "%d", kv->ikey);
            }
        }

        fprintf(fp, "\n");
    }

    return ret;
}

//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
unsigned int p_hashString(const char* s)
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
    // Simple "hash".
    return (unsigned int)(i % DICT_NUM_BINS);
}

//--------------------------------------------------------//
kv_t* p_convertKey(keyType_t kt, key_t k)
{
    // Pack into our preferred format.
    CREATE_INST(kv, kv_t, BAD_PTR);
    if(kt == KEY_STRING)
    {
        CREATE_STR(s, strlen(k.ks), BAD_PTR);
        strcpy(s, k.ks);
        kv->skey = s;
        kv->ikey = 0;
    }
    else
    {
        kv->skey = NULL;
        kv->ikey = k.ki;
    }

    return kv;
}
