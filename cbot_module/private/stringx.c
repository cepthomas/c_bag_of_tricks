
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "common.h"
#include "stringx.h"

/// @brief Definition of string thing.


//---------------- Private --------------------------//

/// Internal data definition.
struct stringx
{
    char* raw;      ///< The owned string. Can be NULL which means empty.
    bool valid;     ///< For lifetime management. Not currently used.
};

/// (Re)assign the underlying char pointer. Takes ownership of the string.
/// @param s Source stringx.
/// @param cs The new raw string or NULL for an empty stringx.
/// @return RS_PASS | RS_ERR.
static int p_Assign(stringx_t* s, char* cs);

/// Case sensitive char matcher.
/// @param c1 First char.
/// @param c2 Second char.
/// @param csens Case sensitivity.
/// @return True if match.
static bool p_Match(char c1, char c2, csens_t csens);

/// Copy a const string.
/// @param sinit String to copy. If NULL, a valid empty string is created.
/// @return The new mutable string | BAD_PTR.
static char* p_Copy(const char* sinit);


//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
stringx_t* stringx_Create(const char* sinit)
{
    VAL_PTR(sinit, BAD_PTR);
    CREATE_INST(s, stringx_t, BAD_PTR);

    // Copy the contents.
    p_Assign(s, p_Copy(sinit));

    return s;
}

//--------------------------------------------------------//
int stringx_Destroy(stringx_t* s)
{
    VAL_PTR(s, RS_ERR);

    int ret = RS_PASS;

    if(s->raw != NULL)
    {
        FREE(s->raw);
        s->raw = NULL;
    }
    FREE(s);

    return ret;
}

//--------------------------------------------------------//
int stringx_Set(stringx_t* s, const char* sinit)
{
    VAL_PTR(s, RS_ERR);
    VAL_PTR(sinit, RS_ERR);

    int ret = RS_PASS;

    // Copy the contents.
    ret = p_Assign(s, p_Copy(sinit));

    return ret;
}

//--------------------------------------------------------//
int stringx_Append(stringx_t* s, stringx_t* sapp)
{
    VAL_PTR(s, RS_ERR);
    VAL_PTR(sapp, RS_ERR);

    int ret = RS_PASS;

    // This is a bit crude. Need to make smarter internal buffer to support growing.
    int slen = stringx_Len(s) + stringx_Len(sapp);
    CREATE_STR(snew, slen, RS_ERR);
    strcpy(snew, s->raw);
    strcpy(snew + stringx_Len(s), sapp->raw);
    ret = p_Assign(s, snew);

    return ret;
}

//--------------------------------------------------------//
const char* stringx_Content(stringx_t* s)
{
    const char* raw = s != NULL ? s->raw : NULL;
    return raw;
}

//--------------------------------------------------------//
int stringx_Len(stringx_t* s)
{
    return s != NULL ? strlen(s->raw) : RS_ERR;
}

//--------------------------------------------------------//
int stringx_Compare(stringx_t* s1, const char* s2, csens_t csens)
{
    VAL_PTR(s1, RS_ERR);
    VAL_PTR(s2, RS_ERR);

    bool match = stringx_Len(s1) == strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_Match(s1->raw[i], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_StartsWith(stringx_t* s1, const char* s2, csens_t csens)
{
    VAL_PTR(s1, RS_ERR);
    VAL_PTR(s2, RS_ERR);

    bool match = stringx_Len(s1) >= strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_Match(s1->raw[i], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_EndsWith(stringx_t* s1, const char* s2, csens_t csens)
{
    VAL_PTR(s1, RS_ERR);
    VAL_PTR(s2, RS_ERR);

    bool match = stringx_Len(s1) >= strlen(s2);
    unsigned int ind1 = stringx_Len(s1) - strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_Match(s1->raw[ind1++], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_Contains(stringx_t* s1, const char* s2, csens_t csens)
{
    VAL_PTR(s1, RS_ERR);
    VAL_PTR(s2, RS_ERR);

    int index = -1;

    // Use strstr to do the hard work.
    if(csens == CASE_SENS)
    {
        char* p = strstr(s1->raw, s2);
        if(p != NULL)
        {
            index = p - s1->raw;
        }
    }
    else
    {
        // Need to convert to lower case before comparing.
        stringx_t* cs1 = stringx_Copy(s1);
        stringx_ToLower(cs1);
        stringx_t* cs2 = stringx_Create(s2);
        stringx_ToLower(cs2);

        char* p = strstr(cs1->raw, cs2->raw);
        if(p != NULL)
        {
            index = p - cs1->raw;
        }
        stringx_Destroy(cs1);
        stringx_Destroy(cs2);
    }

    return index >= 0 ? index : RS_FAIL;
}

//--------------------------------------------------------//
stringx_t* stringx_Copy(stringx_t* s)
{
    VAL_PTR(s, BAD_PTR);

    stringx_t* copy = stringx_Create(s->raw);

    return copy; // could be NULL - OK
}

//--------------------------------------------------------//
stringx_t* stringx_Left(stringx_t* s, unsigned int num)
{
    VAL_PTR(s, BAD_PTR);

    stringx_t* left = stringx_Create("");

    if(strlen(s->raw) >= num)
    {
        CREATE_STR(sleft, num, BAD_PTR);
        CREATE_STR(sresid, stringx_Len(s) - num, BAD_PTR);

        strncpy(sleft, s->raw, num);
        strncpy(sresid, s->raw + num, stringx_Len(s) - num);

        p_Assign(left, sleft);
        p_Assign(s, sresid);
    }

    return left; // could be NULL - OK
}

//--------------------------------------------------------//
int stringx_Trim(stringx_t* s)
{
    VAL_PTR(s, RS_ERR);

    int ret = RS_PASS;

    // __123 456___ len=12

    int first = -1;
    int last = - 1;
    int len = (int)strlen(s->raw);

    // Find first.
    for(int i = 0; first < 0 && i < len; i++)
    {
        if(!isspace(s->raw[i]))
        {
            first = i;
        }
    }

    // Find last.
    for(int i = len - 1; last < 0 && i >= 0; i--)
    {
        if(!isspace(s->raw[i]))
        {
            last = i + 1;
        }
    }

    // Is there work to do?
    if(first >= 0 || last >= 0)
    {
        first = first >= 0 ? first : 0;
        last = last >= 0 ? last : len - 1;

        unsigned int slen = (unsigned int)(last - first);
        CREATE_STR(cs, slen, RS_ERR);
        memcpy(cs, s->raw + first, slen);
        cs[slen] = 0;
        p_Assign(s, cs);
    }

    return ret;
}

//--------------------------------------------------------//
int stringx_ToUpper(stringx_t* s)
{
    VAL_PTR(s, RS_ERR);

    int ret = RS_PASS;

    unsigned int len = strlen(s->raw);

    for(unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)toupper(s->raw[i]);
        }
    }

    return ret;
}

//--------------------------------------------------------//
int stringx_ToLower(stringx_t* s)
{
    VAL_PTR(s, RS_ERR);

    int ret = RS_PASS;

    unsigned int len = strlen(s->raw);

    for(unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)tolower(s->raw[i]);
        }
    }

    return ret;
}

//--------------------------------------------------------//
int stringx_Format(stringx_t* s, unsigned int maxlen, const char* format, ...)
{
    VAL_PTR(s, RS_ERR);
    VAL_PTR(format, RS_ERR);

    int ret = RS_PASS;

    CREATE_STR(buff, maxlen, RS_ERR);
    va_list args;
    va_start(args, format);
    vsnprintf(buff, maxlen, format, args);

    p_Assign(s, buff);
  
    return ret;
}

//--------------------------------------------------------//
list_t* stringx_Split(stringx_t* s, const char* delim)
{
    VAL_PTR(s, BAD_PTR);
    VAL_PTR(delim, BAD_PTR);
   
    list_t* parts = list_Create();
    VAL_PTR(parts, BAD_PTR);

    // Make writable copy and tokenize it.
    CREATE_STR(cp, strlen(s->raw), BAD_PTR);
    strcpy(cp, s->raw);

    char* token = strtok(cp, delim);
    while(token != NULL)
    {
        CREATE_STR(ctoken, strlen(token), BAD_PTR);
        strcpy(ctoken, token);
        list_Append(parts, ctoken);
        token = strtok(NULL, delim);
    }

    FREE(cp);
    return parts;
}

//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
int p_Assign(stringx_t* s, char* cs)
{
    VAL_PTR(s, RS_ERR);
    VAL_PTR(cs, RS_ERR);

    // Flush old.
    if(s->raw != NULL)
    {
        FREE(s->raw);
        s->raw = NULL;
    }

    s->raw = cs == NULL ? NULL : cs;
    // s->valid = true;

    return RS_PASS;
}

//--------------------------------------------------------//
char* p_Copy(const char* sinit)
{
    VAL_PTR(sinit, BAD_PTR);

    char* retbuff;

    CREATE_STR(buff, strlen(sinit), BAD_PTR);
    strcpy(buff, sinit);
    retbuff = buff;

    return retbuff;
}

//--------------------------------------------------------//
bool p_Match(char c1, char c2, csens_t csens)
{
    // A=65 Z=90 a=97 z=122
    bool match = csens == CASE_INSENS ? toupper(c1) == toupper(c2) : c1 == c2;
    return match;
}
