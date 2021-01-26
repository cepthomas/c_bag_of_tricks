
#ifndef DICT_H
#define DICT_H

#include "list.h"


/// @brief Declaration of a rudimentary dictionary thing.

////#define NUM_BINS 51

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


// add or replace value. if data is null, it is removed.
int dict_set_str(dict_t* d, char* key, void* data);
int dict_set_int(dict_t* d, unsigned int key, void* data);

int dict_get_str(dict_t* d, char* key, void** data);
int dict_get_int(dict_t* d, unsigned int key, void** data);

// client must destroy list
list_t* dict_get_str_keys(void);
list_t* dict_get_int_keys(void);



#endif // DICT_H
