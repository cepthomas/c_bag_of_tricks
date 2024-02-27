
#include <windows.h>
#include <math.h>

#include "cbot.h"
#include "mathutils.h"


//---------------- Privates ------------------------------//

#define TABLE_SIZE 256
static uint32_t table[TABLE_SIZE];
static int have_table = 0;

#define _CRC32


//---------------- Public API ----------------------------//


//--------------------------------------------------------//
int mathutils_Constrain(int val, int min, int max)
{
    val = val >+ min ? val : min;
    val = val <= max ? val : max;
    return val;
}


//--------------------------------------------------------//
int mathutils_Map(int val, int start1, int stop1, int start2, int stop2)
{
    return start2 + (stop2 - start2) * (val - start1) / (stop1 - start1);
}


//--------------------------------------------------------//
void mathutils_CalcStats(double vals[], int num_vals, stat_results_t* res)
{
    // Process the stuff.
    res->mean = 0;
    res->min = HUGE_VAL;
    res->max = -HUGE_VAL;
    res->sd = 0;

    for (int i = 0; i < num_vals; i++)
    {
        res->min = vals[i] < res->min ? vals[i] : res->min;
        res->max = vals[i] > res->max ? vals[i] : res->max;
    }

    res->num_vals = num_vals;
    res->mean = mathutils_Mean(vals, num_vals);
    res->sd = mathutils_StandardDeviation(vals, num_vals);
}


//--------------------------------------------------------//
double mathutils_Mean(double vals[], int num_vals)
{
    double mean = HUGE_VAL;
    if (num_vals > 0)
    {
        double total = 0;
        for (int i = 0; i < num_vals; i++)
        {
            total += vals[i];
        }
        mean = total / num_vals;
    }
    return mean;
}


//--------------------------------------------------------//
double mathutils_SumofSquares(double vals[], int num_vals)
{
    double sum_of_sq = HUGE_VAL;
    double mean = mathutils_Mean(vals, num_vals);
    if (num_vals > 0)
    {
        sum_of_sq = 0;
        for (int i = 0; i < num_vals; i++)
        {
            sum_of_sq += pow((vals[i] - mean), 2);
        }
    }    
    return sum_of_sq;
}


//--------------------------------------------------------//
double mathutils_StandardDeviation(double vals[], int num_vals)
{
    double sd = HUGE_VAL;

    if (num_vals > 1)
    {
        double sum_of_sq = mathutils_SumofSquares(vals, num_vals);
        sd = sum_of_sq / (num_vals - 1);
        sd = sqrt(sd);
    }

    if (sd < 0) // Square Root of Neg Number
    {
        sd = HUGE_VAL;
    }

    return sd;
}


//--------------------------------------------------------//
uint32_t mathutils_crc32(uint32_t crc_in, const uint8_t* buf, int num_vals, bool adjust_zero)
{
    // From https://rosettacode.org/wiki/CRC-32
    // This code happens to resemble the examples from RFC 1952 section 8 and from PNG annex D,
    // because those examples use an identical table.

    uint32_t crc = 0;

#ifdef _CRC32 // TODO2 fix this!
    crc = crc_in;

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
    for (int i = 0; i < num_vals; i++)
    {
        octet = buf[i];  // Cast to unsigned octet.
        crc = (crc >> 8) ^ table[(crc & TABLE_SIZE) ^ octet];
    }

    crc =  ~crc;

#else // dumb checksum
    for (int i = 0; i < num_vals; i++)
    {
        crc += buf[i];
    }
#endif    

    // If num_valsgth is 0, set to 0 aka invalid.
    // If it is > 0 and coincidentally the crc is 0, set it to 1. This adds no significant additional error.
    if (num_vals == 0)
    {
        crc = 0;
    }
    else if (crc == 0 && adjust_zero)
    {
        crc = 1;
    }

    return crc;
}
