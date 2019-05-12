
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>


#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif


/// @file

////////// Public interface //////////

/// Generic data payload.
typedef union
{
    void* p;  ///< Generic pointer.
    int i;    ///< Integer.
    char c;   ///< Character.
} listData_t;


/// Opaque list object.
typedef struct list list_t;

/// Create a list. Client must list_destroy() it.
/// @return The opaque pointer used in all functions. 
list_t* list_create();

/// Deletes all nodes. The client is responsible for freeing any malloced data beforehand.
/// @param list List pointer.
void list_clear(list_t* list);

/// Add a node at the beginning.
/// @param list List pointer.
/// @param data Data to add.
void list_push(list_t* list, listData_t data);

/// Add a node at the end.
/// @param list List pointer.
/// @param data Data to add.
void list_append(list_t* list, listData_t data);

/// Remove and return the end. Returns false if at end.
/// @param list List pointer.
/// @param data Where to put the data.
/// @return True if data is valid.
bool list_pop(list_t* list, listData_t* data);

/// Size of the list.
/// @param list List pointer.
/// @return The size. 
int list_count(list_t* list);

/// Initialize iterator.
/// @param list List pointer.
void list_start(list_t* list);

/// Next iteration in list. Returns false if at end.
/// @param list List pointer.
/// @param data Where to put the data.
/// @return True if data is valid.
bool list_next(list_t* list, listData_t* data);

#endif // LIST_H
