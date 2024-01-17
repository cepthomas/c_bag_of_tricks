
#ifndef DICT_H
#define DICT_H

#include "list.h"

/// @brief Declaration of a rudimentary dictionary thing.
/// You can use the value pointers for your own application any way you like.
/// Note that clear() and destroy() will free() them for you but if your data
/// type contains other pointers you will have to manually free those yourself first.


//---------------- Public API ----------------------//

/// Opaque dict object.
typedef struct dict dict_t;

/// Key type per dictionary.
typedef enum { KEY_INT, KEY_STRING } keyType_t;

/// Key for each keyType_t.
typedef union { int ki; const char* ks; } key_t;

/// Create a dict.
/// @param kt Key type.
/// @return The dictionary opaque pointer used in all functions | BAD_PTR. 
dict_t* dict_Create(keyType_t kt);

/// Deletes all nodes and associated data pointers.
/// @param d The dictionary opaque pointer.
/// @return status.
int dict_Clear(dict_t* d);

/// Deletes all nodes and frees associated data pointers, frees the dict struct.
/// @param d The dictionary opaque pointer.
/// @return status.
int dict_Destroy(dict_t* d);

/// Size of the dict.
/// @param d The dict opaque pointer.
/// @return count or error if <0.
int dict_Count(dict_t* d);

/// Set a value using a key. Also used to remove.
/// @param d The dictionary opaque pointer.
/// @param k The key.
/// @param v The value. NOTE value can't contain pointers.
/// @return status.
int dict_Set(dict_t* d, key_t k, void* v);

/// Get a value using a key.
/// @param d The dictionary opaque pointer.
/// @param k The key.
/// @param v Pointer to where to put the associated data.
/// @return status.
int dict_Get(dict_t* d, key_t k, void** v);

/// Get a list of all key_t. NOTE - client must destroy the returned list.
/// @param d The dictionary opaque pointer.
/// @return The list | BAD_PTR.
list_t* dict_GetKeys(dict_t* d);

/// Dump contents of the dict to file.
/// @param d Pertinent dictionary.
/// @param fp Output stream.
/// @return status.
int dict_Dump(dict_t* d, FILE* fp);

#endif // DICT_H
