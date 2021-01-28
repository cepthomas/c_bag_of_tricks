
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "list.h"
#include "dict.h"


/// @brief Definition of dictionary thing.

/// Prime number.
#ifndef DICT_NUM_BINS
#define DICT_NUM_BINS 53
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

//TODO check returns from list funcs.


//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
dict_t* dict_create(keyType_t kt)
{
    CREATE_INST(d, dict_t);
    VALPTR_PTR(d);

    // Initialize.
    d->kt = kt;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* l = list_create();
        VALPTR_PTR(l);
        d->bins[i] = l;
    }

    return d;
}

//--------------------------------------------------------//
int dict_destroy(dict_t* d)
{
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
    int ret = RS_PASS;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        // Remove user data.
        kv_t* kv;
        list_iterStart(pl);
        while(RS_PASS == list_iterNext(pl, (void**)&kv))
        {
            VALPTR_RS(kv);
            if(d->kt == KEY_STRING && kv->key.ks != NULL)
            {
                free(kv->key.ks);
            }
        }
    }

    return ret;
}

//--------------------------------------------------------//
int dict_set(dict_t* d, kv_t* kv)
{
    VALPTR_RS(d);
    VALPTR_RS(kv);
    VALPTR_RS(kv->key.ks);

    int ret = RS_PASS;

    unsigned int bin = d->kt == KEY_STRING ? p_hashString(kv->key.ks) : p_hashInt(kv->key.ki);

    // Is it in the bin already?
    list_t* pl = d->bins[bin]; // shorthand
    list_iterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(RS_PASS == list_iterNext(pl, (void**)&lkv) && !found)
    {
        VALPTR_RS(lkv);

        if(d->kt == KEY_STRING)
        {
            if(strcmp(lkv->key.ks, kv->key.ks) == 0)
            {
                strcpy(lkv->key.ks, kv->key.ks);
                found = true;
            }
        }
        else // KEY_INT
        {
            if(lkv->key.ki == kv->key.ki)
            {
                lkv->key.ki = kv->key.ki;
                found = true;
            }
        }
    }

    if(!found) // new addition
    {
        list_append(pl, (void**)kv);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_get(dict_t* d, kv_t* kv)//, void** data)
{
    int ret = RS_PASS;
    
    return ret;
}
// struct nlist *lookup(char *s)
// {
//     struct nlist *np;
//     for (np = hashtab[hash(s)]; np != NULL; np = np->next)
//         if (strcmp(s, np->name) == 0)
//           return np; /* found */
//     return NULL; /* not found */
// }

//--------------------------------------------------------//
list_t* dict_get_keys(dict_t* d)
{
    
    return BAD_PTR;
}

//--------------------------------------------------------//
int dict_dump(dict_t* d, FILE* fp)
{
    VALPTR_RS(d);
    VALPTR_RS(fp);

    int ret = RS_PASS;

    fprintf(fp, "type,bin,numvals,k0,k1,k2\n");

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        fprintf(fp, "%d,%d,%d", d->kt, i, list_count(pl));


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

        // djb2 revised
        // hash = hash(i - 1) * 33 ^ str[i];

        // sdbm
        // hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return (unsigned int)(hash % DICT_NUM_BINS);
}

//--------------------------------------------------------//
unsigned int p_hashInt(int i)
{
    return (unsigned int)(i % DICT_NUM_BINS);
}
