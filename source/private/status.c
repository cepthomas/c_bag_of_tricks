#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "status.h"


// #define CMN_ERROR_MODULE_LEN                (3)
// #define CMN_ERROR_NUMBER_LEN                (3)
// #define CMN_BYTES_PER_CHAR              3
// #define CMN_UTFSTR(x)                   (CMN_BYTES_PER_CHAR*x)+1
// #define CMN_ASCSTR(x)                   x+1
// #define CMN_DECLARE_UTFSTR(wName, NumChars, wInit) char wName[CMN_UTFSTR(NumChars)] = wInit
// #define CMN_DECLARE_ASCSTR(zName, NumChars, zInit) char zName[CMN_ASCSTR(NumChars)] = zInit
// #define CMN_UTFSPEC(string,characters) utf8strnlen(string,characters,NULL), string

// #define RS_NO_ERR                 0
// #define RS_FAIL_BIT_LOCATION      31
// #define RS_FAIL_BIT_MASK          0x80000000
// #define RS_LOGGED_BIT_LOCATION    30
// #define RS_LOGGED_BIT_MASK        0x40000000
// #define RS_ERROR_MODULE_LOCATION  29
// #define RS_ERROR_MODULE_BITS      16
// #define RS_ERROR_MODULE_BIT_MASK  0x3FFFC000
// #define RS_ERROR_NUMBER_LOCATION  13
// #define RS_ERROR_NUMBER_BITS      14
// #define RS_ERROR_NUMBER_BIT_MASK  0x00003FFF
// #define RS_MODULE_BASE            36
// #define RS_NUMBER_BASE            25

// #define RS_MAKE(fail, logged, smod, num) status_Encode (fail, logged, smod, num)





// byte 1: status - bit7 = error, bit6 = logged, etc...
// byte 2: subsystem - 0xF0 is reserved for cbot, 0x00 is reserved for convenient overlay with other code families.
// byte 3: category - app specific
// byte 4: code - app specific- When errors occur, set errno accordingly.

int status_Encode (bool fail, bool logged, uint8_t subsystem, uint8_t category, uint8_t code)
{
    int stat = STAT_NO_ERR;

    stat = (fail ? 0x80000000 : 0x00000000) + (logged ? 0x40000000 : 0x00000000) + (subsystem << 16) + (category << 8) + (code);
    // stat |= fail ? 0x80000000 : 0x00000000;
    // stat |= logged ? 0x40000000 : 0x00000000;
    // stat |= subsystem << 16;
    // stat |= category << 8;
    // stat |= code;

    return stat;
}
