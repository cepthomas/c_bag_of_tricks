
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "dict.h"


/// @brief Definition of dictionary thing.

/// Prime number.
#ifndef NUM_DICT_BINS
#define NUM_DICT_BINS 53
#endif

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

static key_t p_kt;

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
dict_t* dict_create(key_t kt)
{
    p_kt = kt;

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
    int ret = RET_PASS;
    // dict_clear(d);
    // free(d);
    return ret;
}

//--------------------------------------------------------//
int dict_set(dict_t* d, void* key, void* data)
{
    int ret = RET_PASS;

    return ret;
}

//--------------------------------------------------------//
int dict_get(dict_t* d, void* key, void** data)
{
    int ret = RET_PASS;
    
    return ret;
}

//--------------------------------------------------------//
list_t* dict_get_keys(dict_t* d)
{
    
    return PTR_ERR;
}



/*

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



//////// Taken from http://www.cse.yorku.ca/~oz/hash.html

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
    


sdbm
this algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library. it was found to do well in scrambling bits,
causing better distribution of the keys and fewer splits. it also happens to be a good general hashing function with good distribution.
the actual function is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below is the faster version used in gawk.
there is even a faster, duff-device version] the magic constant 65599 was picked out of thin air while experimenting with different constants,
and turns out to be a prime. this is one of the algorithms used in berkeley db (see sleepycat) and elsewhere.

static unsigned long
sdbm(str)
unsigned char *str;
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

*/


// Section 6.6 of The C Programming Language presents a simple dictionary (hashtable) data structure.

// Note that if the hashes of two strings collide, it may lead to an O(n) lookup time. You can reduce the likelihood of collisions
// by increasing the value of HASHSIZE. For a complete discussion of the data structure, please consult the bo
// Note that the K&R C hashing algorithm is an appalling hash algorithm. See: programmers.stackexchange.com/questions/49550/… 
// for details on how truly terrible is really is.

// struct nlist { /* table entry: */
//     struct nlist *next; /* next entry in chain */
//     char *name; /* defined name */
//     char *defn; /* replacement text */
// };

// #define HASHSIZE 101
// static struct nlist *hashtab[HASHSIZE]; /* pointer table */

// /* hash: form hash value for string s */
// unsigned hash(char *s)
// {
//     unsigned hashval;
//     for (hashval = 0; *s != '\0'; s++)
//       hashval = *s + 31 * hashval;
//     return hashval % HASHSIZE;
// }

// /* lookup: look for s in hashtab */
// struct nlist *lookup(char *s)
// {
//     struct nlist *np;
//     for (np = hashtab[hash(s)]; np != NULL; np = np->next)
//         if (strcmp(s, np->name) == 0)
//           return np; /* found */
//     return NULL; /* not found */
// }

// char *strdup(char *);
// /* install: put (name, defn) in hashtab */
// struct nlist *install(char *name, char *defn)
// {
//     struct nlist *np;
//     unsigned hashval;
//     if ((np = lookup(name)) == NULL) 
//      { /* not found */
//         np = (struct nlist *) malloc(sizeof(*np));
//         if (np == NULL || (np->name = strdup(name)) == NULL)
//           return NULL;
//         hashval = hash(name);
//         np->next = hashtab[hashval];
//         hashtab[hashval] = np;
//     }
//      else /* already there */
//    {
//         free((void *) np->defn); /*free previous defn */
//     }
//     if ((np->defn = strdup(defn)) == NULL)
//        return NULL;
//     return np;
// }

// char *strdup(char *s) /* make a duplicate of s */
// {
//     char *p;
//     p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
//     if (p != NULL)
//        strcpy(p, s);
//     return p;
// }
