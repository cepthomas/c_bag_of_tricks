
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "diagnostics.h"
#include "status.h"
#include "stringx.h"


//---------------- Private Declarations ------------------//

/// Internal data definition.
struct stringx
{
    char* raw;      ///< The owned string. Can be NULL which means empty.
    bool valid;     ///< For lifetime management. Not currently used.
};

/// (Re)assign the underlying char pointer. Takes ownership of the string.
/// @param s Source stringx.
/// @param cs The new raw string or NULL for an empty stringx.
/// @return status.
static int p_Assign(stringx_t* s, char* cs);

/// Case sensitive char matcher.
/// @param c1 First char.
/// @param c2 Second char.
/// @param csens Case sensitivity.
/// @return True if match.
static bool p_Match(char c1, char c2, bool csens);

/// Copy a const string.
/// @param sinit String to copy. If NULL, a valid empty string is created.
/// @return The new mutable string | BAD_PTR.
static char* p_Copy(const char* sinit);


//---------------- Public API Implementation -------------//


//--------------------------------------------------------//
stringx_t* stringx_Create(const char* sinit)
{
    VAL_PTR(sinit, BAD_PTR);
    CREATE_INST(s, stringx_t);

    // Copy the contents.
    p_Assign(s, p_Copy(sinit));

    return s;
}

//--------------------------------------------------------//
int stringx_Destroy(stringx_t* s)
{
    VAL_PTR(s, EARGNULL);

    int stat = ENOERR;

    if(s->raw != NULL)
    {
        FREE(s->raw);
        s->raw = NULL;
    }
    FREE(s);

    return stat;
}

//--------------------------------------------------------//
int stringx_Set(stringx_t* s, const char* sinit)
{
    VAL_PTR(s, EARGNULL);
    VAL_PTR(sinit, EARGNULL);

    int stat = ENOERR;

    // Copy the contents.
    stat = p_Assign(s, p_Copy(sinit));

    return stat;
}

//--------------------------------------------------------//
int stringx_Append(stringx_t* s, stringx_t* sapp)
{
    VAL_PTR(s, EARGNULL);
    VAL_PTR(sapp, EARGNULL);

    int stat = ENOERR;

    // This is a bit crude. Need to make smarter internal buffer to support growing.
    int slen = stringx_Len(s) + stringx_Len(sapp);
    CREATE_STR(snew, slen);
    strcpy(snew, s->raw);
    strcpy(snew + stringx_Len(s), sapp->raw);
    stat = p_Assign(s, snew);

    return stat;
}

//--------------------------------------------------------//
const char* stringx_Content(stringx_t* s)
{
    VAL_PTR(s, BAD_PTR);

    return s->raw;
}

//--------------------------------------------------------//
int stringx_Len(stringx_t* s)
{
    VAL_PTR(s, -EARGNULL);  // negative

    return strlen(s->raw);
}

//--------------------------------------------------------//
int stringx_Compare(stringx_t* s1, const char* s2, bool csens)
{
    VAL_PTR(s1, -EARGNULL);  // negative
    VAL_PTR(s2, -EARGNULL);  // negative

    int ret = 0;
    if (csens)
    {
        ret = strcmp(s1->raw, s2);
    }
    else
    {
        ret = strcasecmp(s1->raw, s2);
    }

    // Impedance match.
    if (ret > 0) ret = 1;
    if (ret < 0) ret = 2;

    return ret;
}

//--------------------------------------------------------//
int stringx_Contains(stringx_t* s1, const char* s2, bool csens)
{
    VAL_PTR(s1, -EARGNULL);  // negative
    VAL_PTR(s2, -EARGNULL);  // negative

    int index = -1;

    // Use strstr to do the hard work.
    if(csens)
    {
        char* p = strstr(s1->raw, s2);
        if(p != NULL)
        {
            index = p - s1->raw;
        }
    }
    else
    {
        // Need to convert to lower case before comparing. //TODO2 kinda clumsy
        stringx_t* lc1 = stringx_Copy(s1);
        stringx_ToLower(lc1);
        stringx_t* lc2 = stringx_Create(s2);
        stringx_ToLower(lc2);

        char* p = strstr(lc1->raw, lc2->raw);
        if(p != NULL)
        {
            index = p - lc1->raw;
        }
        stringx_Destroy(lc1);
        stringx_Destroy(lc2);
    }

    // Match impedance.
    return index >= 0 ? index : -EINVALIDINDEX;
}

//--------------------------------------------------------//
bool stringx_StartsWith(stringx_t* s1, const char* s2, bool csens)
{
    VAL_PTR(s1, false);
    VAL_PTR(s2, false);

    bool match = stringx_Len(s1) >= strlen(s2);

    for (unsigned int i = 0; i < stringx_Len(s1) && i < strlen(s2) && match; i++)
    {
        match = p_Match(s1->raw[i], s2[i], csens);
    }

    return match;
}

//--------------------------------------------------------//
bool stringx_EndsWith(stringx_t* s1, const char* s2, bool csens)
{
    VAL_PTR(s1, false);
    VAL_PTR(s2, false);

    bool match = stringx_Len(s1) >= strlen(s2);
    unsigned int ind1 = stringx_Len(s1) - strlen(s2);

    for (unsigned int i = 0; i < stringx_Len(s1) && i < strlen(s2) && match; i++)
    {
        match = p_Match(s1->raw[ind1++], s2[i], csens);
    }

    return match;
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
        CREATE_STR(sleft, num);
        CREATE_STR(sresid, stringx_Len(s) - num);

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
    VAL_PTR(s, EARGNULL);

    int stat = ENOERR;

    // __123 456___ len=12

    int first = -1;
    int last = - 1;
    int len = (int)strlen(s->raw);

    // Find first.
    for (int i = 0; first < 0 && i < len; i++)
    {
        if(!isspace(s->raw[i]))
        {
            first = i;
        }
    }

    // Find last.
    for (int i = len - 1; last < 0 && i >= 0; i--)
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
        p_Assign(s, cs);
    }

    return stat;
}

//--------------------------------------------------------//
int stringx_ToUpper(stringx_t* s)
{
    VAL_PTR(s, EARGNULL);

    int stat = ENOERR;

    unsigned int len = strlen(s->raw);

    for (unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)toupper(s->raw[i]);
        }
    }

    return stat;
}

//--------------------------------------------------------//
int stringx_ToLower(stringx_t* s)
{
    VAL_PTR(s, EARGNULL);

    int stat = ENOERR;

    unsigned int len = strlen(s->raw);

    for (unsigned int i = 0; i < len; i++)
    {
        if(isalpha(s->raw[i]))
        {
            s->raw[i] = (char)tolower(s->raw[i]);
        }
    }

    return stat;
}

//--------------------------------------------------------//
int stringx_Format(stringx_t* s, unsigned int maxlen, const char* format, ...)
{
    VAL_PTR(s, EARGNULL);
    VAL_PTR(format, EARGNULL);

    int stat = ENOERR;

    CREATE_STR(buff, maxlen);
    va_list args;
    va_start(args, format);
    vsnprintf(buff, maxlen, format, args);

    p_Assign(s, buff);
  
    return stat;
}

//--------------------------------------------------------//
list_t* stringx_Split(stringx_t* s, const char* delim)
{
    VAL_PTR(s, BAD_PTR);
    VAL_PTR(delim, BAD_PTR);
   
    list_t* parts = list_Create();
    VAL_PTR(parts, BAD_PTR);

    // Make writable copy and tokenize it.
    CREATE_STR(cp, strlen(s->raw));
    strcpy(cp, s->raw);

    char* token = strtok(cp, delim);
    while(token != NULL)
    {
        CREATE_STR(ctoken, strlen(token));
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
    VAL_PTR(s, EARGNULL);
    VAL_PTR(cs, EARGNULL);

    // Flush old.
    if(s->raw != NULL)
    {
        FREE(s->raw);
        s->raw = NULL;
    }

    s->raw = cs == NULL ? NULL : cs;
    // s->valid = true;

    return ENOERR;
}

//--------------------------------------------------------//
char* p_Copy(const char* sinit)
{
    VAL_PTR(sinit, BAD_PTR);

    char* retbuff;

    CREATE_STR(buff, strlen(sinit));
    strcpy(buff, sinit);
    retbuff = buff;

    return retbuff;
}

//--------------------------------------------------------//
bool p_Match(char c1, char c2, bool csens)
{
    // A=65 Z=90 a=97 z=122
    bool match = csens ? c1 == c2 : toupper(c1) == toupper(c2);
    return match;
}
