
#ifndef LIST_H
#define LIST_H

/// @brief Declaration of list thing. It's a double linked list implementation.
/// You can use the data pointers for your own application any way you like.
/// Note that clear() and destroy() will free() them for you but if your data
/// type contains other pointers you will have to manually free those yourself first.


//---------------- Public API ----------------------//

/// Opaque list object.
typedef struct list list_t;

/// Create a list.
/// @return The opaque pointer used in all functions | BAD_PTR.
list_t* list_create(void);

/// Deletes all nodes and associated data pointers.
/// @param l The list opaque pointer.
/// @return RS_PASS | RS_ERR.
int list_clear(list_t* l);

/// Deletes all nodes and frees associated data pointers, frees the list struct.
/// @param l The list opaque pointer.
/// @return RS_PASS | RS_ERR.
int list_destroy(list_t* l);

/// Add a node at the beginning.
/// @param l The list opaque pointer.
/// @param data Data to add. NOTE can't contain pointers.
/// @return RS_PASS | RS_ERR.
int list_push(list_t* l, void* data);

/// Add a node at the end.
/// @param l The list opaque pointer.
/// @param data Data to add. NOTE can't contain pointers.
/// @return RS_PASS | RS_ERR.
int list_append(list_t* l, void* data);

/// Remove and return the end. Returns false if at end.
/// @param l The list opaque pointer.
/// @param data Where to put the data. Client takes ownership of it now!
/// @return RS_PASS | RS_ERR | RS_FAIL.
int list_pop(list_t* l, void** data);

/// Size of the list.
/// @param l The list opaque pointer.
/// @return The size | RS_ERR.
int list_count(list_t* l);

/// Initialize iterator.
/// @param l The list opaque pointer.
/// @return RS_PASS | RS_ERR | RS_FAIL (if empty).
int list_iterStart(list_t* l);

/// Next iteration in list.
/// @param l The list opaque pointer.
/// @param data Where to put the data.
/// @return RS_PASS | RS_ERR | RS_FAIL (if empty or at end)
int list_iterNext(list_t* l, void** data);

#endif // LIST_H
