
#ifndef DICT_H
#define DICT_H

#include "list.h"
// Optionally define NUM_DICT_BINS as some-prime-number
#include "dict.h"


/// @brief Declaration of a rudimentary dictionary thing.


//---------------- Public API ----------------------//

/// Opaque dict object.
typedef struct dict dict_t;

/// Supported key types.
typedef enum { KEY_INT, KEY_STRING } key_t;

/// Create a dict.
/// @param kt Key type.
/// @return The dictionary opaque pointer used in all functions | PTR_ERR. 
dict_t* dict_create(key_t kt);

/// Deletes all nodes and associated data pointers.
/// @param d The dictionary opaque pointer.
/// @return RET_PASS | RET_ERR.
int dict_clear(dict_t* d);

/// Deletes all nodes and frees associated data pointers, and the dict struct.
/// @param d The dictionary opaque pointer.
/// @return RET_PASS | RET_ERR.
int dict_destroy(dict_t* d);

/// Set a value using a key. Also used to remove.
/// @param d The dictionary opaque pointer.
/// @param key The key.
/// @param data Pointer to the associated data, or remove key if NULL.
/// @return RET_PASS | RET_ERR.
int dict_set(dict_t* d, void* key, void* data);

/// Get a value using a key.
/// @param d The dictionary opaque pointer.
/// @param key The key.
/// @param data Pointer to where to put the associated data.
/// @return RET_PASS | RET_FAIL | RET_ERR.
int dict_get(dict_t* d, void* key, void** data);

/// Get a list of all keys. NOTE - client must destroy the returned list.
/// @param d The dictionary opaque pointer.
/// @param key The key.
/// @param data Pointer to where to put the associated data.
/// @return RET_PASS | RET_FAIL | RET_ERR.
list_t* dict_get_keys(dict_t* d);

#endif // DICT_H
