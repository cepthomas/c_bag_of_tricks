
#include <stdlib.h>
#include <stdio.h>

#include "diagnostics.h"
#include "status.h"
#include "list.h"


//---------------- Private Declarations ------------------//

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
list_t* list_Create(void)
{
    CREATE_INST(l, list_t);

    return l;
}

//--------------------------------------------------------//
int list_Destroy(list_t* l)
{
    VAL_PTR(l, EARGNULL);

    int ret = list_Clear(l);
    FREE(l);

    return ret;
}

//--------------------------------------------------------//
int list_Clear(list_t* l)
{
    VAL_PTR(l, EARGNULL);

    int ret = ENOERR;

    // Remove all nodes and corresponding data.
    node_t* iter = l->head;
    while(iter != NULL)
    {
        node_t* next = iter->next;
        if(iter->data != NULL)
        {
            FREE(iter->data);
            iter->data = NULL;
        }
        FREE(iter);
        iter = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->iter = NULL;

    return ret;
}

//--------------------------------------------------------//
int list_Push(list_t* l, void* data)
{
    VAL_PTR(l, EARGNULL);
    VAL_PTR(data, EARGNULL);

    int ret = ENOERR;

    CREATE_INST(new_node, node_t);
    new_node->data = data;

    // Get current head. Could be null if empty.
    node_t* chead = l->head;

    if(chead != NULL) // insert
    {
        chead->prev = new_node;
        new_node->next = chead;
        l->head = new_node;
    }
    else // init
    {
        l->head = new_node;
    }
    
    // Clean up tail also.
    if(l->tail == NULL)
    {
        l->tail = new_node;
    }

    return ret;
}

//--------------------------------------------------------//
int list_Append(list_t* l, void* data)
{
    VAL_PTR(l, EARGNULL);
    VAL_PTR(data, EARGNULL);

    int ret = ENOERR;

    // Get current tail. Can be null.
    node_t* ctail = l->tail;

    CREATE_INST(new_node, node_t);
    new_node->data = data;

    if(ctail != NULL)
    {
        ctail->next = new_node;
    }

    new_node->next = NULL; // last
    new_node->prev = ctail;

    l->tail = new_node;

    // Init if first.
    if(l->head == NULL)
    {
        l->head  = new_node;
    }

    return ret;
}

//--------------------------------------------------------//
int list_Pop(list_t* l, void** data)
{
    VAL_PTR(l, EARGNULL);
    VAL_PTR(data, EARGNULL);

    int ret = ENOERR;

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
        FREE(ctail);
        ctail = NULL;
    }
    else // no data there
    {
        ret = EITEREND;
    }

    return ret;
}

//--------------------------------------------------------//
int list_Count(list_t* l)
{
    VAL_PTR(l, MAKE_FAIL_ERRNO(EARGNULL));

    int cnt = 0;
    node_t* iter = l->head;
    while(iter != NULL)
    {
        cnt++;
        iter = iter->next;
    }

    return cnt;
}

//--------------------------------------------------------//
int list_IterStart(list_t* l)
{
    VAL_PTR(l, EARGNULL);

    int ret = ENOERR;

    l->iter = l->head;

    node_t* nt = l->iter;
    if(nt == NULL)
    {
        ret = EITEREND;
    }

    return ret;
}

//--------------------------------------------------------//
int list_IterNext(list_t* l, void** data)
{
    VAL_PTR(l, EARGNULL);
    VAL_PTR(data, EARGNULL);

    int ret = ENOERR;

    node_t* nt = l->iter;
    if(nt != NULL)
    {
        l->iter = nt->next;
        *data = nt->data;
    }
    else
    {
        ret = EITEREND;
    }

    return ret;
}
