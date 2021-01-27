
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "list.h"


/// @brief Definition of list thing.

//---------------- Private --------------------------//

/// List node definition.
typedef struct node
{
    void* data;         ///< Client specific data. Client must cast.
    struct node* prev;  ///< Linked list previous node.
    struct node* next;  ///< Linked list next node.
} node_t;

/// Double linked list definition.
struct list
{
    node_t* head;       ///< Linked list head.
    node_t* tail;       ///< Linked list tail.
    node_t* iter;       ///< Internal pointer for iteration operations.
};

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
list_t* list_create(void)
{
    CREATE_INST(pl, list_t);
    VALIDATE_PTR1(pl, PTR_ERR);

    return pl;
}

//--------------------------------------------------------//
int list_destroy(list_t* pl)
{
    VALIDATE_PTR1(pl, RET_ERR);

    int ret = list_clear(pl);
    free(pl);

    return ret;
}

//--------------------------------------------------------//
int list_push(list_t* pl, void* data)
{
    VALIDATE_PTR2(pl, data, RET_ERR);

    int ret = RET_PASS;

    CREATE_INST(newNode, node_t);
    VALIDATE_PTR1(newNode, RET_ERR);

    newNode->data = data;

    // Get current head. Could be null if empty.
    node_t* chead = pl->head;

    if(chead != NULL) // insert
    {
        chead->prev = newNode;
        newNode->next = chead;
        pl->head = newNode;
    }
    else // init
    {
        pl->head = newNode;
    }
    
    // Clean up tail also.
    if(pl->tail == NULL)
    {
        pl->tail = newNode;
    }

    return ret;
}

//--------------------------------------------------------//
int list_append(list_t* pl, void* data)
{
    VALIDATE_PTR2(pl, data, RET_ERR);

    int ret = RET_PASS;

    // Get current tail. Can be null.
    node_t* ctail = pl->tail;

    CREATE_INST(newNode, node_t);
    VALIDATE_PTR1(newNode, RET_ERR);

    newNode->data = data;

    if(ctail != NULL)
    {
        ctail->next = newNode;
    }

    newNode->next = NULL; // last
    newNode->prev = ctail;

    pl->tail = newNode;

    // Init.
    if(pl->head == NULL)
    {
        pl->head  = newNode;
    }

    return ret;
}

//--------------------------------------------------------//
int list_pop(list_t* pl, void** data)
{
    VALIDATE_PTR2(pl, *data, RET_ERR);

    int ret = RET_PASS;

    // Get current tail.
    node_t* ctail = pl->tail;

    if(ctail != NULL)
    {
        // Return the attached data.
        *data = ctail->data;

        // Update neighbors.
        if(ctail->prev == NULL) // special processing for the first element
        {
           pl->head = NULL;
           pl->tail = NULL;
        }
        else
        {
           pl->tail = ctail->prev;
           pl->tail->next = NULL;
        }

        // Remove the node.
        free(ctail);
        ctail = NULL;
    }
    else // no data there
    {
        ret = RET_FAIL;
    }

    return ret;
}

//--------------------------------------------------------//
int list_count(list_t* pl)
{
    VALIDATE_PTR1(pl, RET_ERR);

    int ret = RET_PASS;

    ret = 0;
    node_t* iter = pl->head;
    while(iter != NULL)
    {
        ret++;
        iter = iter->next;
    }

    return ret;
}

//--------------------------------------------------------//
int list_start(list_t* pl)
{
    VALIDATE_PTR1(pl, RET_ERR);

    int ret = RET_PASS;

    pl->iter = pl->head;

    return ret;
}

//--------------------------------------------------------//
int list_next(list_t* pl, void** data)
{
    VALIDATE_PTR2(pl, *data, RET_ERR);

    int ret = RET_PASS;

    node_t* nt = pl->iter;
    if(nt != NULL)
    {
        pl->iter = nt->next;
        *data = nt->data;
    }
    else
    {
        ret = RET_FAIL;
    }

    return ret;
}

//--------------------------------------------------------//
int list_clear(list_t* pl)
{
    VALIDATE_PTR1(pl, RET_ERR);

    int ret = RET_PASS;

    // Remove all nodes and corresponding data.
    node_t* iter = pl->head;
    while(iter != NULL)
    {
        node_t* next = iter->next;
        if(iter->data != NULL)
        {
            free(iter->data);
            iter->data = NULL;
        }
        free(iter);
        iter = next;
    }

    pl->head = NULL;
    pl->tail = NULL;
    pl->iter = NULL;

    return ret;
}
