
#ifndef DICT_H
#define DICT_H

#include "list.h"
// Optionally define DICT_NUM_BINS as some-prime-number here.
#include "dict.h"


/// @brief Declaration of a rudimentary dictionary thing.


//---------------- Public API ----------------------//

/// Opaque dict object.
typedef struct dict dict_t;

/// Supported key types.
typedef enum { KEY_INT, KEY_STRING } keyType_t;

/// Key for each keyType_t.
typedef union { int ki; char* ks; } key_t;

/// Key-value pair.
typedef struct kv
{
    key_t key;      ///> The key.
    void* value;    ///> Client specific data. Client must cast.
} kv_t;

/// Create a dict.
/// @param kt Key type.
/// @return The dictionary opaque pointer used in all functions | BAD_PTR. 
dict_t* dict_create(keyType_t kt);

/// Deletes all nodes and associated data pointers.
/// @param d The dictionary opaque pointer.
/// @return RET_PASS | RET_ERR.
int dict_clear(dict_t* d);

/// Deletes all nodes and frees associated data pointers, frees the dict struct.
/// @param d The dictionary opaque pointer.
/// @return RET_PASS | RET_ERR.
int dict_destroy(dict_t* d);

// /// Set a value using a key. Also used to remove.
// /// @param d The dictionary opaque pointer.
// /// @param key The key.
// /// @param data Pointer to the associated data, or remove key if NULL.
// /// @return RET_PASS | RET_ERR.
// int dict_set(dict_t* d, void* key, void* data);

/// Set a value using a key. Also used to remove.
/// @param d The dictionary opaque pointer.
/// @param kv The key-value pair.
/// @return RET_PASS | RET_ERR.
int dict_set(dict_t* d, kv_t* kv);

/// Get a value using a key.
/// @param d The dictionary opaque pointer.
/// @param kv The key.
/// @param data Pointer to where to put the associated data.
/// @return RET_PASS | RET_FAIL | RET_ERR.
int dict_get(dict_t* d, kv_t* kv);//, void** data);

/// Get a list of all key_t. NOTE - client must destroy the returned list.
/// @param d The dictionary opaque pointer.
/// @param key The key.
/// @param data Pointer to where to put the associated data.
/// @return RET_PASS | RET_FAIL | RET_ERR.
list_t* dict_get_keys(dict_t* d);

/// Dump contents of the dict to file.
/// @param d Pertinent dictionary.
/// @param fp Output stream.
/// @return RET_PASS | RET_ERR.
int dict_dump(dict_t* d, FILE* fp);

#endif // DICT_H
