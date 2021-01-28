
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "list.h"
#include "dict.h"


/// @brief Definition of dictionary thing.

/// Prime number.
#ifndef NUM_DICT_BINS
#define NUM_DICT_BINS 53
#endif


//---------------- Private --------------------------//

/// Bin definition.
typedef struct binItem
{

    // void* data;         ///< Client specific data. Client must cast.
    // struct node* prev;  ///< Linked list previous node.
    // struct node* next;  ///< Linked list next node.

} binItem_t;


struct dict
{
    list_t* bins[NUM_DICT_BINS];

    // node_t* head;  ///< Linked list head.
    // node_t* tail;  ///< Linked list tail.
    // node_t* iter;  ///< Internal pointer for iteration operations.

};



static key_t p_kt;


static unsigned int p_hash(char* s);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
dict_t* dict_create(key_t kt)
{
    p_kt = kt;

    CREATE_INST(d, dict_t);
    VALIDATE_PTR1(d, RET_PTR_ERR);

    // initialize...
    // d->head = NULL;
    // d->tail = NULL;
    // d->iter = NULL;

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

    p_hash("999");

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
    
    return RET_PTR_ERR;
}

//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
unsigned int p_hash(char* s)
{
    // Lifted from http://www.cse.yorku.ca/~oz/hash.html.
    unsigned long hash = 5381;
    // bool done = false;
    int c;

    while ((c = *s++))
    {
        // djb2
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 
        // djb2 revised
        // hash = hash(i - 1) * 33 ^ str[i];
        // sdbm
        // hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return (unsigned int)(hash % NUM_DICT_BINS);
}


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
