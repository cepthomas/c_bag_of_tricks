
#ifndef LIST_H
#define LIST_H

#include "stdbool.h"


/// @brief Declaration of list thing.

//---------------- Public API ----------------------//

/// Opaque list object.
typedef struct list list_t;

/// Create a list.
/// @return The opaque pointer used in all functions. 
list_t* list_create();

/// Deletes all nodes and associated data pointers.
/// @param list List pointer.
void list_clear(list_t* list);

/// Deletes all nodes and frees associated data pointers, and the list struct.
/// @param list List pointer.
void list_destroy(list_t* list);

/// Add a node at the beginning.
/// @param list List pointer.
/// @param data Data to add.
void list_push(list_t* list, void* data);

/// Add a node at the end.
/// @param list List pointer.
/// @param data Data to add.
void list_append(list_t* list, void* data);

/// Remove and return the end. Returns false if at end.
/// @param list List pointer.
/// @param data Where to put the data pointer. Client takes ownership of it now!
/// @return True if data is valid.
bool list_pop(list_t* list, void** data);

/// Size of the list.
/// @param list List pointer.
/// @return The size. 
int list_count(list_t* list);

/// Initialize iterator.
/// @param list List pointer.
void list_start(list_t* list);

/// Next iteration in list.
/// @param list List pointer.
/// @param data Where to put the data pointer.
/// @return True if data is valid. Returns false if at end.
bool list_next(list_t* list, void** data);

#endif // LIST_H
