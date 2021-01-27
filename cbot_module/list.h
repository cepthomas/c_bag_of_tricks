
#ifndef LIST_H
#define LIST_H

// #include <stdbool.h>


/// @brief Declaration of list thing. It's a double linked list implementation.

 // TODO all the void** and void* casts?? Also in dict.


//---------------- Public API ----------------------//

/// Opaque list object.
typedef struct list list_t;

/// Create a list.
/// @return The opaque pointer used in all functions | PTR_ERR.
list_t* list_create(void);

/// Deletes all nodes and associated data pointers.
/// @param l The list opaque pointer.
/// @return RET_PASS | RET_ERR.
int list_clear(list_t* l);

/// Deletes all nodes and frees associated data pointers, and the list struct.
/// @param l The list opaque pointer.
/// @return RET_PASS | RET_ERR.
int list_destroy(list_t* l);

/// Add a node at the beginning.
/// @param l The list opaque pointer.
/// @param data Data to add.
/// @return RET_PASS | RET_ERR.
int list_push(list_t* l, void* data);

/// Add a node at the end.
/// @param l The list opaque pointer.
/// @param data Data to add.
/// @return RET_PASS | RET_ERR.
int list_append(list_t* l, void* data);

/// Remove and return the end. Returns false if at end.
/// @param l The list opaque pointer.
/// @param data Where to put the data. Client takes ownership of it now!
/// @return RET_PASS | RET_ERR | RET_FAIL.
int list_pop(list_t* l, void** data);

/// Size of the list.
/// @param l The list opaque pointer.
/// @return The size | RET_ERR.
int list_count(list_t* l);

/// Initialize iterator.
/// @param l The list opaque pointer.
/// @return RET_PASS | RET_ERR.
int list_start(list_t* l);

/// Next iteration in list.
/// @param l The list opaque pointer.
/// @param data Where to put the data.
/// @return RET_PASS | RET_ERR | RET_FAIL (if at end)
int list_next(list_t* l, void** data);

#endif // LIST_H
