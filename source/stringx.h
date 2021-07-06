
#ifndef STRINGX_H
#define STRINGX_H

#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "list.h"


/// @brief Declaration of string thing.


//---------------- Public API ----------------------//

/// Case sensitivity for ops.
typedef enum
{
    CASE_SENS,
    CASE_INSENS
} csens_t;

/// Opaque string object.
typedef struct stringx stringx_t;

/// Create an empty string.
/// @param sinit Initial value. If empty use "".
/// @return The opaque pointer used in all functions | BAD_PTR.
stringx_t* stringx_Create(const char* sinit);

/// Frees all data pointers, and the string struct.
/// @param s Source stringx. After this returns it is no longer valid.
/// @return RS_PASS | RS_ERR.
int stringx_Destroy(stringx_t* s);

/// Get the contained data.
/// @param s Source stringx.
/// @return The char pointer | BAD_PTR.
const char* stringx_Content(stringx_t* s);

/// Size of the string.
/// @param s Source stringx.
/// @return The size | RS_ERR.
int stringx_Len(stringx_t* s);

/// Set s to new value.
/// @param s Source stringx.
/// @param sinit The new value. If NULL, content will be "".
/// @return RS_PASS | RS_ERR.
int stringx_Set(stringx_t* s, const char* sinit);

/// Convert to upper case IN PLACE.
/// @param s Source stringx.
/// @return RS_PASS | RS_ERR.
int stringx_ToUpper(stringx_t* s);

/// Convert to lower case IN PLACE.
/// @param s Source stringx.
/// @return RS_PASS | RS_ERR.
int stringx_ToLower(stringx_t* s);

/// Compare strings.
/// @param s1 Source stringx.
/// @param s2 The test value.
/// @param csens Case sensitivity.
/// @return RS_PASS if equal | RS_FAIL if not | RS_ERR.
int stringx_Compare(stringx_t* s1, const char* s2, csens_t csens);

/// Test if string starts with.
/// @param s1 Source stringx.
/// @param s2 The test value.
/// @param csens Case sensitivity.
/// @return RS_PASS if true | RS_FAIL if not | RS_ERR.
int stringx_StartsWith(stringx_t* s1, const char* s2, csens_t csens);

/// Test if string ends with.
/// @param s1 Source stringx.
/// @param s2 The test value.
/// @param csens Case sensitivity.
/// @return RS_PASS if true | RS_FAIL if not | RS_ERR.
int stringx_EndsWith(stringx_t* s1, const char* s2, csens_t csens);

/// Test if string contains.
/// @param s1 Source stringx.
/// @param s2 The test value.
/// @param csens Case sensitivity.
/// @return Index of match | RS_ERR.
int stringx_Contains(stringx_t* s1, const char* s2, csens_t csens);

/// Copy a stringx.
/// @param s Source stringx.
/// @return The copied string | BAD_PTR.
stringx_t* stringx_Copy(stringx_t* s);

/// Removes left chars as a new stringx.
/// @param s Source stringx.
/// @param num Number to remove.
/// @return The left part or empty if num > len(s) | BAD_PTR
stringx_t* stringx_Left(stringx_t* s, unsigned int num);

/// Trim whitespace from both ends IN PLACE.
/// @param s Source stringx.
/// @return RS_PASS | RS_ERR.
int stringx_Trim(stringx_t* s);

/// Append a string to the stringx.
/// @param s Source stringx.
/// @param sapp String to append.
/// @return RS_PASS | RS_ERR.
int stringx_Append(stringx_t* s, stringx_t* sapp);

/// Format the string IN PLACE.
/// @param s Source stringx.
/// @param maxlen Client must give us a clue.
/// @param format Standard format string and args.
/// @return RS_PASS | RS_ERR. Not used right now but in future could check arg validity.
int stringx_Format(stringx_t* s, unsigned int maxlen, const char* format, ...);

/// Split the string into parts by token.
/// @param s Source stringx.
/// @param delim Like strtok.
/// @return List of string parts | BAD_PTR.
list_t* stringx_Split(stringx_t* s, const char* delim);

#endif // STRINGX_H
