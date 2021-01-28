
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
    CREATE_INST(l, list_t);
    VALIDATE_PTR1(l, RET_PTR_ERR);

    return l;
}

//--------------------------------------------------------//
int list_destroy(list_t* l)
{
    VALIDATE_PTR1(l, RET_ERR);

    int ret = list_clear(l);
    free(l);

    return ret;
}

//--------------------------------------------------------//
int list_push(list_t* l, void* data)
{
    VALIDATE_PTR2(l, data, RET_ERR);

    int ret = RET_PASS;

    CREATE_INST(newNode, node_t);
    VALIDATE_PTR1(newNode, RET_ERR);

    newNode->data = data;

    // Get current head. Could be null if empty.
    node_t* chead = l->head;

    if(chead != NULL) // insert
    {
        chead->prev = newNode;
        newNode->next = chead;
        l->head = newNode;
    }
    else // init
    {
        l->head = newNode;
    }
    
    // Clean up tail also.
    if(l->tail == NULL)
    {
        l->tail = newNode;
    }

    return ret;
}

//--------------------------------------------------------//
int list_append(list_t* l, void* data)
{
    VALIDATE_PTR2(l, data, RET_ERR);

    int ret = RET_PASS;

    // Get current tail. Can be null.
    node_t* ctail = l->tail;

    CREATE_INST(newNode, node_t);
    VALIDATE_PTR1(newNode, RET_ERR);

    newNode->data = data;

    if(ctail != NULL)
    {
        ctail->next = newNode;
    }

    newNode->next = NULL; // last
    newNode->prev = ctail;

    l->tail = newNode;

    // Init.
    if(l->head == NULL)
    {
        l->head  = newNode;
    }

    return ret;
}

//--------------------------------------------------------//
int list_pop(list_t* l, void** data)
{
    VALIDATE_PTR2(l, data, RET_ERR);

    int ret = RET_PASS;

    // Get current tail.
    node_t* ctail = l->tail;

    if(ctail != NULL)
    {
        // Return the attached data.
        *data = ctail->data;

        // Update neighbors.
        if(ctail->prev == NULL) // special processing for the first element
        {
           l->head = NULL;
           l->tail = NULL;
        }
        else
        {
           l->tail = ctail->prev;
           l->tail->next = NULL;
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
int list_count(list_t* l)
{
    VALIDATE_PTR1(l, RET_ERR);

    int ret = RET_PASS;

    ret = 0;
    node_t* iter = l->head;
    while(iter != NULL)
    {
        ret++;
        iter = iter->next;
    }

    return ret;
}

//--------------------------------------------------------//
int list_start(list_t* l)
{
    VALIDATE_PTR1(l, RET_ERR);

    int ret = RET_PASS;

    l->iter = l->head;

    return ret;
}

//--------------------------------------------------------//
int list_next(list_t* l, void** data)
{
    VALIDATE_PTR2(l, data, RET_ERR);

    int ret = RET_PASS;

    node_t* nt = l->iter;
    if(nt != NULL)
    {
        l->iter = nt->next;
        *data = nt->data;
    }
    else
    {
        ret = RET_FAIL;
    }

    return ret;
}

//--------------------------------------------------------//
int list_clear(list_t* l)
{
    VALIDATE_PTR1(l, RET_ERR);

    int ret = RET_PASS;

    // Remove all nodes and corresponding data.
    node_t* iter = l->head;
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

    l->head = NULL;
    l->tail = NULL;
    l->iter = NULL;

    return ret;
}
