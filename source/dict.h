
#ifndef DICT_H
#define DICT_H


/// @brief Declaration of a dictionary thing.


//---------------- Public API ----------------------//

/// Opaque dict object.
typedef struct dict dict_t;

/// Create a dict.
/// @return The opaque pointer used in all functions | PTR_ERR. 
dict_t* dict_create(void);

/// Deletes all nodes and associated data pointers.
/// @param d dict pointer.
/// @return RET_PASS | RET_ERR.
int dict_clear(dict_t* d);

/// Deletes all nodes and frees associated data pointers, and the dict struct.
/// @param d dict pointer.
/// @return RET_PASS | RET_ERR.
int dict_destroy(dict_t* d);



int dict_add_str(dict_t* d, char* key, void* data);
int dict_add_int(dict_t* d, unsigned int key, void* data);
int dict_get_str(dict_t* d, char* key, void** data);
int dict_get_int(dict_t* d, unsigned int key, void** data);
int dict_remove_str(dict_t* d, char* key);
int dict_remove_int(dict_t* d, unsigned int key);
// get keys


#endif // DICT_H
