#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#include "chksum.h"


//---------------- Private Declarations ------------------//

#define TABLE_SIZE 256
static uint32_t table[TABLE_SIZE];
static int have_table = 0;

#define _CRC32

//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
uint32_t chksum_crc32(uint32_t crc_in, const uint8_t* buf, size_t len, bool adjust_zero)
{
    // From https://rosettacode.org/wiki/CRC-32
    // This code happens to resemble the examples from RFC 1952 section 8 and from PNG annex D,
    // because those examples use an identical table.

    uint32_t crc = 0;

#ifdef _CRC32 // TODO2 fix this!
    crc = crc_in;

    // This check is not thread safe; there is no mutex.
    if (have_table == 0)
    {
        // Calculate CRC table.
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            uint32_t rem = i;  // remainder from polynomial division
            for (int j = 0; j < 8; j++)
            {
                if (rem & 1)
                {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                {
                    rem >>= 1;
                }
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;

    uint8_t octet;
    for (int i = 0; i < len; i++)
    {
        octet = buf[i];  // Cast to unsigned octet.
        crc = (crc >> 8) ^ table[(crc & TABLE_SIZE) ^ octet];
    }

    crc =  ~crc;

#else // dumb checksum
    for (int i = 0; i < len; i++)
    {
        crc += buf[i];
    }
#endif    

    // If length is 0, set to 0 aka invalid.
    // If it is > 0 and coincidentally the crc is 0, set it to 1. This adds no significant additional error.
    if (len == 0)
    {
        crc = 0;
    }
    else if (crc == 0 && adjust_zero)
    {
        crc = 1;
    }

    return crc;
}

//---------------- Private Implementation -------------//

