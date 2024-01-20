
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "diagnostics.h"
#include "cbot.h"
#include "cbot_internal.h"
#include "list.h"
#include "dict.h"


//---------------- Private Declarations ------------------//

/// Size should be a prime number somewhere around the quantity of items.
#define DICT_NUM_BINS 101

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
static unsigned int p_HashString(const char* s);

/// Make hash from int and bin it.
/// @param i The int.
/// @return Hash value between 0 and DICT_NUM_BINS.
static unsigned int p_HashInt(int i);

/// Convert client to internal format.
/// @param kt Type.
/// @param k Key itself.
/// @return New pointer to thing.
static kv_t* p_ConvertKey(keyType_t kt, key_t k);

//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
dict_t* dict_Create(keyType_t kt)
{
    CREATE_INST(d, dict_t);

    // Initialize.
    d->kt = kt;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* l = list_Create();
        VAL_PTR(l, BAD_PTR);
        d->bins[i] = l;
    }

    return d;
}

//--------------------------------------------------------//
int dict_Destroy(dict_t* d)
{
    VAL_PTR(d, CBOT_ERR_ARG_NULL);

    int ret = CBOT_ERR_NO_ERR;

    // Clean up user data.
    dict_Clear(d);

    // Remove all bins.
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_Destroy(d->bins[i]);
    }

    FREE(d);

    return ret;
}

//--------------------------------------------------------//
int dict_Clear(dict_t* d)
{
    VAL_PTR(d, CBOT_ERR_ARG_NULL);

    int ret = CBOT_ERR_NO_ERR;

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        // Remove custom user data.
        kv_t* kv;
        list_IterStart(pl);

        while(list_IterNext(pl, (void**)&kv) == CBOT_ERR_NO_ERR)
        {
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

            // kv gets freed in list_clear() below.
        }

        ret = list_Clear(pl);
        if (ret != CBOT_ERR_NO_ERR)
        {
            break; // hosed, return now.
        }
    }

    return ret;
}

//--------------------------------------------------------//
int dict_Count(dict_t* d)
{
    VAL_PTR(d, -CBOT_ERR_ARG_NULL); // negative

    int cnt = 0;
    
    for(int i = 0; i < DICT_NUM_BINS && cnt >= 0; i++)
    {
        int bcnt = list_Count(d->bins[i]);
        if(bcnt >= 0)
        {
            cnt += bcnt;
        }
        else // error, bail out.
        {
            cnt = bcnt;
            break;
        }
    }

    return cnt;
}

//--------------------------------------------------------//
int dict_Set(dict_t* d, key_t k, void* v)
{
    VAL_PTR(d, CBOT_ERR_ARG_NULL);
    VAL_PTR(v, CBOT_ERR_ARG_NULL);

    int ret = CBOT_ERR_NO_ERR;

    // If it is in a bin already, replace the value.
    unsigned int bin = d->kt == KEY_STRING ? p_HashString(k.ks) : p_HashInt(k.ki);
    list_t* pl = d->bins[bin]; // shorthand

    list_IterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(list_IterNext(pl, (void**)&lkv) == CBOT_ERR_NO_ERR && !found)
    {
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
            // Need to FREE the original data then copy from the new.
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
        kv_t* kv = p_ConvertKey(d->kt, k);
        kv->value = v;

        list_Append(pl, (void*)kv);
    }

    return ret;
}

//--------------------------------------------------------//
int dict_Get(dict_t* d, key_t k, void** v)
{
    VAL_PTR(d, CBOT_ERR_ARG_NULL);

    int ret = CBOT_ERR_INVALID_INDEX;

    // Is it in the bin?
    unsigned int bin = d->kt == KEY_STRING ? p_HashString(k.ks) : p_HashInt(k.ki);
    list_t* pl = d->bins[bin]; // shorthand

    list_IterStart(pl);
    kv_t* lkv;
    bool found = false;

    while(list_IterNext(pl, (void**)&lkv) == CBOT_ERR_NO_ERR && !found)
    {
        if(d->kt == KEY_STRING)
        {
            if(strcmp(lkv->skey, k.ks) == 0)
            {
                ret = CBOT_ERR_NO_ERR;
                *v = lkv->value;
                found = true;
            }
        }
        else // KEY_INT
        {
            if(lkv->ikey == k.ki)
            {
                ret = CBOT_ERR_NO_ERR;
                *v = lkv->value;
                found = true;
            }
        }
    }

    return ret;
}

//--------------------------------------------------------//
list_t* dict_GetKeys(dict_t* d)
{
    VAL_PTR(d, BAD_PTR);

    list_t* l = list_Create();
    VAL_PTR(l, BAD_PTR);
    
    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand

        list_IterStart(pl);

        kv_t* kv;

        while(list_IterNext(pl, (void**)&kv) == CBOT_ERR_NO_ERR)
        {
            if(d->kt == KEY_STRING)
            {
                // Copy only.
                CREATE_STR(s, strlen(kv->skey));
                strcpy(s, kv->skey);
                list_Append(l, s);
            }
            else // KEY_INT
            {
                CREATE_INST(pi, int);
                list_Append(l, pi);
            }
        }
    }

    return l;
}

//--------------------------------------------------------//
int dict_Dump(dict_t* d, FILE* fp)
{
    VAL_PTR(d, CBOT_ERR_ARG_NULL);
    VAL_PTR(fp, CBOT_ERR_ARG_NULL);

    int ret = CBOT_ERR_NO_ERR;

    // Preamble.
    fprintf(fp, "type,bins,total\n");
    int cnt = dict_Count(d);
    if (cnt < 0)
    {
        fprintf(fp, "Bad count:%d\n", -cnt);
        return -cnt;
    }
    fprintf(fp, "%d,%d,%d\n\n", d->kt, DICT_NUM_BINS, cnt);

    // Content.
    fprintf(fp, "bin,num,key0,key1,key2\n");

    for(int i = 0; i < DICT_NUM_BINS; i++)
    {
        list_t* pl = d->bins[i]; // shorthand
        cnt = list_Count(pl);
        if (cnt < 0)
        {
            fprintf(fp, "Bad count:%d\n", -cnt);
            return -cnt;
        }

        fprintf(fp, "%d,%d", i, cnt);

        list_IterStart(pl);

        for(int k = 0; k < (int)fmin(cnt, 3); k++)
        {
            fprintf(fp, ",");
            kv_t* kv;
            list_IterNext(pl, (void**)&kv);

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
unsigned int p_HashString(const char* s)
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
unsigned int p_HashInt(int i)
{
    // Simple "hash".
    return (unsigned int)(i % DICT_NUM_BINS);
}

//--------------------------------------------------------//
kv_t* p_ConvertKey(keyType_t kt, key_t k)
{
    // Pack into our preferred format.
    CREATE_INST(kv, kv_t);
    if(kt == KEY_STRING)
    {
        CREATE_STR(s, strlen(k.ks));
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
