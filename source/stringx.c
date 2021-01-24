
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
    char* raw;   ///< The owned string. Can be NULL which means empty.
    //bool valid;  ///< For lifetime management.
};

/// (Re)assign the underlying char pointer. Takes ownership of the string.
/// @param s Source stringx.
/// @param cs The new raw string or NULL for an empty stringx.
static void p_assign(stringx_t* s, char* cs);

/// Case sensitive char matcher.
/// @param c1 First char.
/// @param c2 Second char.
/// @param csens Case sensitivity.
/// @return True if they match.
static bool p_match(char c1, char c2, csens_t csens);

/// Copy a const string.
/// @param sinit String to copy. If NULL, a valid empty string is created.
/// @return The new mutable string.
static char* p_scopy(const char* sinit);


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
stringx_t* stringx_create(const char* sinit)
{
    CREATE_INST(s, stringx_t); //TODO all checks!!! + CREATE_STR
    s->raw = NULL;
    // Copy the contents.
    p_assign(s, p_scopy(sinit));
    return s;
}

//--------------------------------------------------------//
void stringx_destroy(stringx_t* s)
{
    if(s != NULL)
    {
        if(s->raw != NULL)
        {
            free(s->raw);
            s->raw = NULL;
        }
        free(s);
    }
}

//--------------------------------------------------------//
void stringx_set(stringx_t* s, const char* sinit)
{
    // Copy the contents.
    p_assign(s, p_scopy(sinit));
}

//--------------------------------------------------------//
void stringx_append(stringx_t* s, stringx_t* sapp)
{
    // This is a bit crude. Need to make smarter internal buffer to support growing.
    unsigned int slen = stringx_len(s) + stringx_len(sapp);
    CREATE_STR(snew, slen);
    strcpy(snew, s->raw);
    strcpy(snew + stringx_len(s), sapp->raw);
    p_assign(s, snew);
}

//--------------------------------------------------------//
const char* stringx_content(stringx_t* s)
{
    const char* raw = s != NULL ? s->raw : NULL;
    return raw;
}

//--------------------------------------------------------//
unsigned int stringx_len(stringx_t* s)
{
    unsigned int len = s != NULL ? strlen(s->raw) : 0;
    return len;
}

//--------------------------------------------------------//
bool stringx_compare(stringx_t* s1, const char* s2, csens_t csens)
{
    bool match = stringx_len(s1) == strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[i], s2[i], csens);
    }

    return match;
}

//--------------------------------------------------------//
bool stringx_startswith(stringx_t* s1, const char* s2, csens_t csens)
{
    bool match = stringx_len(s1) >= strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[i], s2[i], csens);
    }

    return match;
}

//--------------------------------------------------------//
bool stringx_endswith(stringx_t* s1, const char* s2, csens_t csens)
{
    bool match = stringx_len(s1) >= strlen(s2);
    unsigned int ind1 = stringx_len(s1) - strlen(s2);

    for(unsigned int i = 0; i < strlen(s2) && match; i++)
    {
        match = p_match(s1->raw[ind1++], s2[i], csens);
    }

    return match;
}

//--------------------------------------------------------//
int stringx_contains(stringx_t* s1, const char* s2, csens_t csens)
{
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

    return index;
}

//--------------------------------------------------------//
stringx_t* stringx_copy(stringx_t* s)
{
    stringx_t* copy = stringx_create(s->raw);
    return copy;
}

//--------------------------------------------------------//
stringx_t* stringx_left(stringx_t* s, unsigned int num)
{
    stringx_t* left = stringx_create(NULL);

    if(strlen(s->raw) >= num)
    {
        CREATE_STR(sleft, num);
        CREATE_STR(sresid, stringx_len(s) - num);

        strncpy(sleft, s->raw, num);
        strncpy(sresid, s->raw + num, stringx_len(s) - num);

        p_assign(left, sleft);
        p_assign(s, sresid);
    }

    return left;
}

//--------------------------------------------------------//
void stringx_trim(stringx_t* s)
{
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
        memcpy(cs, s->raw + first, slen);
        cs[slen] = 0;
        p_assign(s, cs);
    }
}

//--------------------------------------------------------//
void stringx_toupper(stringx_t* s)
{
    unsigned int len = strlen(s->raw);

    for(unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)toupper(s->raw[i]);
        }
    }
}

//--------------------------------------------------------//
void stringx_tolower(stringx_t* s)
{
    unsigned int len = strlen(s->raw);

    for(unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)tolower(s->raw[i]);
        }
    }
}

//--------------------------------------------------------//
bool stringx_format(stringx_t* s, unsigned int maxlen, const char* format, ...)
{
    bool stat = true;

    CREATE_STR(buff, maxlen);
    va_list args;
    va_start(args, format);
    vsnprintf(buff, maxlen, format, args);

    p_assign(s, buff);
  
    return stat;
}

#ifdef HAVE_LIST
//--------------------------------------------------------//
list_t* stringx_split(stringx_t* s, const char* delim)
{
    list_t* parts = list_create();

    // Make writable copy and tokenize it.
    CREATE_STR(cp, strlen(s->raw));
    strcpy(cp, s->raw);

    char* token = strtok(cp, delim);
    while(token != NULL)
    {
        CREATE_STR(ctoken, strlen(token));
        strcpy(ctoken, token);
        list_append(parts, ctoken);
        token = strtok(NULL, delim);
    }

    free(cp);
    return parts;
}
#endif

//---------------- Private Implementation --------------------------//

void p_assign(stringx_t* s, char* cs)
{
    if(s->raw != NULL)
    {
        free(s->raw);
    }

    s->raw = cs == NULL ? NULL : cs;
//    s->valid = true;
}

//--------------------------------------------------------//
char* p_scopy(const char* sinit)
{
    char* retbuff;

    // Copy the contents.
    if(sinit != NULL)
    {
        CREATE_STR(buff, strlen(sinit));
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
    //A=65 Z=90 a=97 z=122
    bool match = csens == CASE_INSENS ? toupper(c1) == toupper(c2) : c1 == c2;
    return match;
}
