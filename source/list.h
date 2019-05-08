
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>


#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif


////////// Client facing interface //////////

/// Generic data payload.
typedef union
{
    void* p;
    int i;
    char c;
} listData_t;


/// Opaque list object.
typedef struct list list_t;

/// Create a list. Client must list_destroy() it.
list_t* list_create();

/// Deletes all nodes. The client is responsible for freeing any malloced data beforehand.
void list_clear(list_t* list);

/// Add a node at the beginning.
void list_push(list_t* list, listData_t data);

/// Add a node at the end.
void list_append(list_t* list, listData_t data);

/// Remove and return the end. Returns false if at end.
bool list_pop(list_t* list, listData_t* data);

/// Size of the list.
int list_count(list_t* list);

/// Returns the first in list. Does not remove it. Returns false if invalid.
bool list_first(list_t* list, listData_t* data);

/// Initialize iterator.
void list_start(list_t* list);

/// Next iteration in list. Returns false if at end.
bool list_next(list_t* list, listData_t* data);

#endif // LIST_H
