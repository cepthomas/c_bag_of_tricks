
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
static int p_assign(stringx_t* s, char* cs);

/// Case sensitive char matcher.
/// @param c1 First char.
/// @param c2 Second char.
/// @param csens Case sensitivity.
/// @return True if match.
static bool p_match(char c1, char c2, csens_t csens);

/// Copy a const string.
/// @param sinit String to copy. If NULL, a valid empty string is created.
/// @return The new mutable string | BAD_PTR.
static char* p_scopy(const char* sinit);


//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
stringx_t* stringx_create(const char* sinit)
{
    CREATE_INST(s, stringx_t);
    VALPTR_PTR(s);

    // Copy the contents.
    p_assign(s, p_scopy(sinit));

    return s;
}

//--------------------------------------------------------//
int stringx_destroy(stringx_t* s)
{
    VALPTR_RS(s);

    int ret = RS_PASS;

    if(s->raw != NULL)
    {
        free(s->raw);
        s->raw = NULL;
    }
    free(s);

    return ret;
}

//--------------------------------------------------------//
int stringx_set(stringx_t* s, const char* sinit)
{
    VALPTR_RS(s);
    VALPTR_RS(sinit);

    int ret = RS_PASS;

    // Copy the contents.
    ret = p_assign(s, p_scopy(sinit));

    return ret;
}

//--------------------------------------------------------//
int stringx_append(stringx_t* s, stringx_t* sapp)
{
    VALPTR_RS(s);
    VALPTR_RS(sapp);

    int ret = RS_PASS;

    // This is a bit crude. Need to make smarter internal buffer to support growing.
    int slen = stringx_len(s) + stringx_len(sapp);
    CREATE_STR(snew, slen);
    VALPTR_RS(snew);
    strcpy(snew, s->raw);
    strcpy(snew + stringx_len(s), sapp->raw);
    ret = p_assign(s, snew);

    return ret;
}

//--------------------------------------------------------//
const char* stringx_content(stringx_t* s)
{
    const char* raw = s != NULL ? s->raw : NULL;
    return raw;
}

//--------------------------------------------------------//
int stringx_len(stringx_t* s)
{
    return s != NULL ? strlen(s->raw) : RS_ERR;
}

//--------------------------------------------------------//
int stringx_compare(stringx_t* s1, const char* s2, csens_t csens)
{
    VALPTR_RS(s1);
    VALPTR_RS(s2);

    bool match = stringx_len(s1) == strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[i], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_startswith(stringx_t* s1, const char* s2, csens_t csens)
{
    VALPTR_RS(s1);
    VALPTR_RS(s2);

    bool match = stringx_len(s1) >= strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[i], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_endswith(stringx_t* s1, const char* s2, csens_t csens)
{
    VALPTR_RS(s1);
    VALPTR_RS(s2);

    bool match = stringx_len(s1) >= strlen(s2);
    unsigned int ind1 = stringx_len(s1) - strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[ind1++], s2[i], csens);
    }

    return match ? RS_PASS : RS_FAIL;
}

//--------------------------------------------------------//
int stringx_contains(stringx_t* s1, const char* s2, csens_t csens)
{
    VALPTR_RS(s1);
    VALPTR_RS(s2);

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
        stringx_t* cs1 = stringx_copy(s1);
        stringx_tolower(cs1);
        stringx_t* cs2 = stringx_create(s2);
        stringx_tolower(cs2);

        char* p = strstr(cs1->raw, cs2->raw);
        if(p != NULL)
        {
            index = p - cs1->raw;
        }
        stringx_destroy(cs1);
        stringx_destroy(cs2);
    }

    return index >= 0 ? index : RS_FAIL;
}

//--------------------------------------------------------//
stringx_t* stringx_copy(stringx_t* s)
{
    VALPTR_PTR(s);

    stringx_t* copy = stringx_create(s->raw);

    return copy; // could be NULL - OK
}

//--------------------------------------------------------//
stringx_t* stringx_left(stringx_t* s, unsigned int num)
{
    VALPTR_PTR(s);

    stringx_t* left = stringx_create("");

    if(strlen(s->raw) >= num)
    {
        CREATE_STR(sleft, num);
        CREATE_STR(sresid, stringx_len(s) - num);
        VALPTR_PTR(sleft);
        VALPTR_PTR(sresid);

        strncpy(sleft, s->raw, num);
        strncpy(sresid, s->raw + num, stringx_len(s) - num);

        p_assign(left, sleft);
        p_assign(s, sresid);
    }

    return left; // could be NULL - OK
}

//--------------------------------------------------------//
int stringx_trim(stringx_t* s)
{
    VALPTR_RS(s);

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
        CREATE_STR(cs, slen);
        VALPTR_RS(cs);
        memcpy(cs, s->raw + first, slen);
        cs[slen] = 0;
        p_assign(s, cs);
    }

    return ret;
}

//--------------------------------------------------------//
int stringx_toupper(stringx_t* s)
{
    VALPTR_RS(s);

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
int stringx_tolower(stringx_t* s)
{
    VALPTR_RS(s);

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
int stringx_format(stringx_t* s, unsigned int maxlen, const char* format, ...)
{
    VALPTR_RS(s);
    VALPTR_RS(format);

    int ret = RS_PASS;

    CREATE_STR(buff, maxlen);
    VALPTR_RS(buff);
    va_list args;
    va_start(args, format);
    vsnprintf(buff, maxlen, format, args);

    p_assign(s, buff);
  
    return ret;
}

//--------------------------------------------------------//
list_t* stringx_split(stringx_t* s, const char* delim)
{
    VALPTR_PTR(s);
    VALPTR_PTR(delim);
   
    list_t* parts = list_create();
    VALPTR_PTR(parts);

    // Make writable copy and tokenize it.
    CREATE_STR(cp, strlen(s->raw));
    VALPTR_PTR(cp);
    strcpy(cp, s->raw);

    char* token = strtok(cp, delim);
    while(token != NULL)
    {
        CREATE_STR(ctoken, strlen(token));
        VALPTR_PTR(ctoken);
        strcpy(ctoken, token);
        list_append(parts, ctoken);
        token = strtok(NULL, delim);
    }

    free(cp);
    return parts;
}

//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
int p_assign(stringx_t* s, char* cs)
{
    VALPTR_RS(s);
    VALPTR_RS(cs);

    // Flush old.
    if(s->raw != NULL)
    {
        free(s->raw);
    }

    s->raw = cs == NULL ? NULL : cs;
    // s->valid = true;

    return RS_ERR;
}

//--------------------------------------------------------//
char* p_scopy(const char* sinit)
{
    VALPTR_PTR(sinit);

    char* retbuff;

    // Copy the contents.
    if(sinit != NULL)
    {
        CREATE_STR(buff, strlen(sinit));
        VALPTR_PTR(buff);
        strcpy(buff, sinit);
        retbuff = buff;
    }
    else // make it a valid empty string
    {
        retbuff = (char*)calloc(1, sizeof(char));
    }

    return retbuff;
}

//--------------------------------------------------------//
bool p_match(char c1, char c2, csens_t csens)
{
    // A=65 Z=90 a=97 z=122
    bool match = csens == CASE_INSENS ? toupper(c1) == toupper(c2) : c1 == c2;
    return match;
}
