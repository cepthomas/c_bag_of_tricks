
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "list.h"


/// @file Definition of list thing.

//---------------- Private --------------------------//

/// List node definition.
typedef struct node
{
    void* data;         ///< Client specific data. Client must cast.
    struct node* prev;  ///< Linked list previous node.
    struct node* next;  ///< Linked list next node.
} node_t;

/// Doubly-linked list definition.
struct list
{
    node_t* head;  ///< Linked list head.
    node_t* tail;  ///< Linked list tail.
    node_t* iter;  ///< Internal pointer for iteration operations.
};

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
list_t* list_create()
{
    CREATE_INST(list, list_t);
    list->head = NULL;
    list->tail = NULL;
    list->iter = NULL;
    return list;
}

//--------------------------------------------------------//
void list_push(list_t* list, void* data)
{
    if(list != NULL)
    {
        // Get current head. Can be null.
        node_t* chead = list->head;

        CREATE_INST(newNode, node_t);
        newNode->data = data;

        if(chead != NULL)
        {
            chead->prev = newNode;
        }

        newNode->prev = NULL; // first
        newNode->next = chead;

        list->head = newNode;

        // Init.
        if(list->tail == NULL)
        {
            list->tail = newNode;
        }
    }
}

//--------------------------------------------------------//
void list_append(list_t* list, void* data)
{
    if(list != NULL)
    {
        // Get current tail. Can be null.
        node_t* ctail = list->tail;

        CREATE_INST(newNode, node_t);
        newNode->data = data;

        if(ctail != NULL)
        {
            ctail->next = newNode;
        }

        newNode->next = NULL; // last
        newNode->prev = ctail;

        list->tail = newNode;

        // Init.
        if(list->head == NULL)
        {
            list->head  = newNode;
        }
    }
}

//--------------------------------------------------------//
bool list_pop(list_t* list, void** data)
{
    bool ret = false;

    if(list != NULL)
    {
        // Get current tail.
        node_t* ctail = list->tail;

        if(ctail != NULL)
        {
            // Return the attached data.
            *data =  ctail->data;
            ret = true;

            // Remove the node.
            if(ctail->prev == NULL)
            {
               list->head = ctail->next;
            }
            else
            {
               ctail->prev->next = ctail->next;
            }

            if(ctail->next == NULL)
            {
               list->tail = ctail->prev;
            }
            else
            {
               ctail->next->prev = ctail->prev;
            }

            // Remove the node.
            free(ctail);
            ctail = NULL;
        }
    }

    return ret;
}

//--------------------------------------------------------//
int list_count(list_t* list)
{
    int i = 0;

    if(list != NULL)
    {
        node_t* iter = list->head;
        while(iter != NULL)
        {
            i++;
            iter = iter->next;
        }
    }

    return i;
}

//--------------------------------------------------------//
void list_start(list_t* list)
{
    if(list != NULL)
    {
        list->iter = list->head;
    }
}

//--------------------------------------------------------//
bool list_next(list_t* list, void** data)
{
    bool ret = false;

    if(list != NULL)
    {
        node_t* nt = list->iter;
        if(nt != NULL)
        {
            list->iter = nt->next;
            *data = nt->data;
            ret = true;
        }
    }

    return ret;
}

//--------------------------------------------------------//
void list_clear(list_t* list)
{
    if(list != NULL)
    {
        // Remove all nodes and corresponding data.
        node_t* iter = list->head;
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

        list->head = NULL;
        list->tail = NULL;
        list->iter = NULL;
    }
}

//--------------------------------------------------------//
void list_destroy(list_t* list)
{
    list_clear(list);
    free(list);
}
