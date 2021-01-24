
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



#endif // DICT_H
