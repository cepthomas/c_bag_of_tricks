#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// This file contains the definition of the return status used throughout Oasis code.
// Originally this was a class, but was changed to an integer for performance reasons.

// Bit composition of a RET_STAT
// * 31 Fail bit               
// * 30 Logged bit             
// * 29-14 Encoded Error module
// * 13-0 Encoded error number

typedef signed int RET_STAT;
#define CMN_ERROR_MODULE_LEN                (3)
#define CMN_ERROR_NUMBER_LEN                (3)
#define CMN_BYTES_PER_CHAR              3
#define CMN_UTFSTR(x)                   (CMN_BYTES_PER_CHAR*x)+1
#define CMN_ASCSTR(x)                   x+1
#define CMN_DECLARE_UTFSTR(wName, NumChars, wInit) char wName[CMN_UTFSTR(NumChars)] = wInit
#define CMN_DECLARE_ASCSTR(zName, NumChars, zInit) char zName[CMN_ASCSTR(NumChars)] = zInit
#define CMN_UTFSPEC(string,characters) utf8strnlen(string,characters,NULL), string 

#define RS_NO_ERR                 0
#define RS_FAIL_BIT_LOCATION      31
#define RS_FAIL_BIT_MASK          0x80000000
#define RS_LOGGED_BIT_LOCATION    30
#define RS_LOGGED_BIT_MASK        0x40000000
#define RS_ERROR_MODULE_LOCATION  29
#define RS_ERROR_MODULE_BITS      16
#define RS_ERROR_MODULE_BIT_MASK  0x3FFFC000
#define RS_ERROR_NUMBER_LOCATION  13
#define RS_ERROR_NUMBER_BITS      14
#define RS_ERROR_NUMBER_BIT_MASK  0x00003FFF
#define RS_MODULE_BASE            36
#define RS_NUMBER_BASE            25

#define RS_MAKE(fail, logged, smod, num) CMN_RetStatEncode (fail, logged, smod, num)

#define RS_MAKE_PASS(smod, num) RS_MAKE (false, false, smod, num)

#define RS_MAKE_FAIL( smod, num) RS_MAKE (true, false, smod, num)

#define RS_PASS(rs) ((((rs) & RS_FAIL_BIT_MASK) == 0) ? true : false)

#define RS_FAIL(rs) ((((rs) & RS_FAIL_BIT_MASK) == 0) ? false : true)

#define RS_ERR_LOGGED(rs) ((((rs) & RS_LOGGED_BIT_MASK) == 0) ? false : true)

#define RS_ERR_PENDING(rs) ((RS_FAIL(rs)) && (!(RS_ERR_LOGGED(rs))))

#define RS_SET_FAIL(rs, fail) (rs = (((((int)fail << RS_FAIL_BIT_LOCATION)) & (RS_FAIL_BIT_MASK)) | (rs)))

#define RS_SET_LOGGED(rs, logged) (rs = ((((int)logged << RS_LOGGED_BIT_LOCATION) & (RS_LOGGED_BIT_MASK)) | (rs)))

RET_STAT CMN_RetStatEncode (bool bFail, bool bLogged, const char* pzErrorModule, const char* pzErrorNumber);
void CMN_RetStatDecode (RET_STAT RetStat, bool* pbFail, bool* pbLogged, char* pzErrorModule, char* pzErrorNumber);
const char* FormatRetStatTest(RET_STAT RetStat);



///////////////////////// Logging? ///////////////////////////
const int MAX_LOG_LINE = 1024;

RET_STAT ES_LogError(RET_STAT* pErr, const char* zFname, const int iLineNum, const char* zFmt, ...)
{
    char zStr[MAX_LOG_LINE];

    // Create the varargs string.
    va_list args;
    va_start(args, zFmt);
    vsnprintf(zStr, MAX_LOG_LINE, zFmt, args);
    va_end(args);

//    qDebug() << "*** " << FormatRetStatTest(*pErr) << " " << zStr; // << " File:" << zFname << " Line:" << iLineNum;

    return RS_NO_ERR;
}

const char* FormatRetStatTest(RET_STAT RetStat)
{
    static char zBuff[20];
    memset(zBuff, 0, 20);

    if(RetStat == RS_NO_ERR)
    {
        sprintf(zBuff, "RS_NO_ERR");
    }
    else
    {
        bool bFail;
        bool bLogged;
        char zErrorModule[CMN_ASCSTR(CMN_ERROR_MODULE_LEN)];
        char zErrorNumber[CMN_ASCSTR(CMN_ERROR_NUMBER_LEN)];
        CMN_RetStatDecode(RetStat, &bFail, &bLogged, zErrorModule, zErrorNumber);
        sprintf(zBuff, "%s-%s", zErrorModule, zErrorNumber);
    }

    return zBuff;
}

RET_STAT CMN_RetStatEncode(bool bFail, bool bLogged, const char* pzErrorModule, const char* pzErrorNumber)
{
    RET_STAT cRetStat = RS_NO_ERR;
    int iEncodedErrorModule = 0;
    int iEncodedErrorNumber = 0;
    char zCharacter = 0;
    int iMappedCharacter = 0;
    int i = 0;

    if(bFail == true)
    {
        cRetStat = cRetStat | RS_FAIL_BIT_MASK;
    }

    if(bLogged == true)
        cRetStat = cRetStat | RS_LOGGED_BIT_MASK;

    if(pzErrorModule[0] != 0)
    {
        for (i = 0; i < CMN_ERROR_MODULE_LEN; i++)
        {
            zCharacter = pzErrorModule[CMN_ERROR_MODULE_LEN - 1 - i];
            if (isupper((int)zCharacter))
                iMappedCharacter = zCharacter - 'A' + ('9' - '0') + 1;
            else
                iMappedCharacter = zCharacter - '0';
            iEncodedErrorModule += (iMappedCharacter * (int)pow(RS_MODULE_BASE,(double)i));
        }

        cRetStat =  cRetStat | (((iEncodedErrorModule << (RS_ERROR_MODULE_LOCATION - RS_ERROR_MODULE_BITS + 1)) & RS_ERROR_MODULE_BIT_MASK));
    }

    if(pzErrorNumber[0] != 0)
    {
        for(i = 0; i < CMN_ERROR_NUMBER_LEN; i++)
        {
            zCharacter = pzErrorNumber[CMN_ERROR_NUMBER_LEN - 1 - i];

            if (isupper((int)zCharacter))
                iMappedCharacter = zCharacter - 'A' + ('9' - '0') + 1;
            else
                iMappedCharacter = zCharacter - '0';
            iEncodedErrorNumber += (iMappedCharacter * (int)pow(RS_NUMBER_BASE,(double)i));
        }
        cRetStat =  cRetStat | (((iEncodedErrorNumber << (RS_ERROR_NUMBER_LOCATION - RS_ERROR_NUMBER_BITS + 1)) & RS_ERROR_NUMBER_BIT_MASK));
    }

    return cRetStat;
}

void CMN_RetStatDecode(RET_STAT cRetStat, bool* pbFail, bool* pbLogged, char* pzErrorModule, char* pzErrorNumber)
{
    int iEncodedErrorModule = 0;
    int iEncodedErrorNumber = 0;
    int iMappedCharacter = 0;
    int i = 0;

    *pbFail = RS_FAIL(cRetStat);
    *pbLogged = RS_ERR_LOGGED(cRetStat);
    iEncodedErrorModule = ((cRetStat & RS_ERROR_MODULE_BIT_MASK) >> (RS_ERROR_MODULE_LOCATION - RS_ERROR_MODULE_BITS + 1));
    iEncodedErrorNumber = ((cRetStat & RS_ERROR_NUMBER_BIT_MASK) >> (RS_ERROR_NUMBER_LOCATION - RS_ERROR_NUMBER_BITS + 1));

    for (i = CMN_ERROR_MODULE_LEN; i > 0; i--)
    {
        iMappedCharacter = (iEncodedErrorModule % (const int)(pow(RS_MODULE_BASE,(double)(CMN_ERROR_MODULE_LEN - i + 1)))) /
                (int)(pow(RS_MODULE_BASE,(double)(CMN_ERROR_MODULE_LEN - i)));

        iEncodedErrorModule = iEncodedErrorModule - iMappedCharacter;

        if (iMappedCharacter > ('9' - '0'))
            pzErrorModule[i - 1] = iMappedCharacter - ('9' - '0') + 'A' - 1;
        else
            pzErrorModule[i - 1] = iMappedCharacter + '0';
    }

    pzErrorModule[CMN_ERROR_MODULE_LEN] = 0;

    for (i = CMN_ERROR_NUMBER_LEN; i > 0; i--)
    {
        iMappedCharacter =  (iEncodedErrorNumber % (const int)(pow(RS_NUMBER_BASE,(double)(CMN_ERROR_NUMBER_LEN - i + 1)))) /
                (int)(pow(RS_NUMBER_BASE,(double)(CMN_ERROR_NUMBER_LEN - i)));

        iEncodedErrorNumber = iEncodedErrorNumber - iMappedCharacter;

        if (iMappedCharacter > ('9' - '0'))
            pzErrorNumber[i - 1] = iMappedCharacter - ('9' - '0') + 'A' - 1;
        else
            pzErrorNumber[i - 1] = iMappedCharacter + '0';
    }

    pzErrorNumber[CMN_ERROR_MODULE_LEN] = 0;
}
