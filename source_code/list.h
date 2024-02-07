
#ifndef LIST_H
#define LIST_H

/// A rudimentary list thing. It's a double linked list implementation.
/// You can use the data pointers for your own application any way you like.
/// Note that clear() and destroy() will free() them for you but if your data
/// type contains other pointers you will have to manually free those yourself first.


//---------------- Public API ----------------------//

/// Opaque list object.
typedef struct list list_t;

/// Create a list.
/// @return The opaque pointer used in all functions | BAD_PTR.
list_t* list_Create(void);

/// Deletes all nodes and associated data pointers.
/// @param l The list opaque pointer.
/// @return status.
int list_Clear(list_t* l);

/// Deletes all nodes and frees associated data pointers, frees the list struct.
/// @param l The list opaque pointer.
/// @return status.
int list_Destroy(list_t* l);

/// Add a node at the beginning.
/// @param l The list opaque pointer.
/// @param data Data to add. NOTE can't contain pointers.
/// @return status.
int list_Push(list_t* l, void* data);

/// Add a node at the end.
/// @param l The list opaque pointer.
/// @param data Data to add. NOTE can't contain pointers.
/// @return status.
int list_Append(list_t* l, void* data);

/// Remove and return the end.
/// @param l The list opaque pointer.
/// @param data Where to put the data. Client takes ownership of it now!
/// @return status.
int list_Pop(list_t* l, void** data);

/// Size of the list.
/// @param l The list opaque pointer.
/// @return count | negative errno.
int list_Count(list_t* l);

/// Initialize iterator.
/// @param l The list opaque pointer.
/// @return status.
int list_IterStart(list_t* l);

/// Next iteration in list.
/// @param l The list opaque pointer.
/// @param data Where to put the data.
/// @return status.
int list_IterNext(list_t* l, void** data);

#endif // LIST_H
