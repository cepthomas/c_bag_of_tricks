
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "dict.h"


/// @brief Definition of list thing.

//---------------- Private --------------------------//

// /// List node definition.
// typedef struct node
// {
//     void* data;         ///< Client specific data. Client must cast.
//     struct node* prev;  ///< Linked list previous node.
//     struct node* next;  ///< Linked list next node.
// } node_t;

// /// Doubly-linked list definition.
// struct list
// {
//     node_t* head;  ///< Linked list head.
//     node_t* tail;  ///< Linked list tail.
//     node_t* iter;  ///< Internal pointer for iteration operations.
// };

struct dict
{
    // node_t* head;  ///< Linked list head.
    // node_t* tail;  ///< Linked list tail.
    // node_t* iter;  ///< Internal pointer for iteration operations.
};

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
dict_t* dict_create(void)
{
    CREATE_INST(d, dict_t);
    if(d != NULL)
    {
        // initialize...

        // d->head = NULL;
        // d->tail = NULL;
        // d->iter = NULL;
    }
    else
    {
        // rs.code = SC_YYY;
        // rs.extra = __LINE__;
    }

    return d;
}

//--------------------------------------------------------//
int dict_clear(dict_t* d)
{
    int ret = RET_ERR;

    return ret;
}

//--------------------------------------------------------//
int dict_destroy(dict_t* d)
{
    int ret = RET_ERR;
    dict_clear(d);
    free(d);
    return ret;
}


/////////////////////////////////////////////
/*
static const int32_t DS_HASHMAP_BUCKETS = 1000000;

#define DS_HASHMAP_KEY_INT 1
#define DS_HASHMAP_KEY_STRING 2

struct DSHashMap {
    // storing the keys isn't strictly necessary for a hash map, but it makes
    // iterating over the elements in a hash map much more efficient.
    struct DSVector *keys;
    struct DSHashItem **buckets;
};

struct DSHashItem {
    struct DSHashKey *key;
    void *data;
    struct DSHashItem *next;
};

struct DSHashKey {
    struct DSHashMap *hash; // useful to avoid global scoping a hash 
    int8_t keytype;
    union {
        int32_t i;
        char* s;
    } key;
};

djb2
this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. 
another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i];
the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
    unsigned long
    hash(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; // hash * 33 + c 

        return hash;
    }
    

// Taken from http://www.cse.yorku.ca/~oz/hash.html
static uint64_t
hash_string(char* str)
{
    uint64_t hash = 5381;
    char c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash % DS_HASHMAP_BUCKETS;
}

static uint64_t
hash_int(int32_t integer)
{
    return (uint64_t) integer % DS_HASHMAP_BUCKETS;
}

struct DSHashMap *
ds_hashmap_create()
{
    struct DSHashMap *hash;
    int32_t i;

    hash = malloc(sizeof(*hash));
    assert(hash);

    hash->keys = ds_vector_create();
    hash->buckets = malloc(DS_HASHMAP_BUCKETS * sizeof(*hash->buckets));
    assert(hash->buckets);

    for (i = 0; i < DS_HASHMAP_BUCKETS; ++i)
        hash->buckets[i] = NULL;

    return hash;
}

*/