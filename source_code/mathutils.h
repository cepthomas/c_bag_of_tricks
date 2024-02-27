#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>


/// Analysis results.
typedef struct stat_results
{
    int num_vals;
    double mean;
    double min;
    double max;
    double sd;
} stat_results_t;


/// Bounds limits a value.
/// @param val
/// @param min
/// @param max
/// @return constrained value.
int mathutils_Constrain(int val, int min, int max)
{
    val = val >+ min ? val : min;
    val = val <= max ? val : max;
    return val;
}


/// Remap a value to new coordinates.
/// @param val
/// @param start1
/// @param stop1
/// @param start2
/// @param stop2
/// <returns></returns>
/// @return mapped value.
int mathutils_Map(int val, int start1, int stop1, int start2, int stop2)
{
    return start2 + (stop2 - start2) * (val - start1) / (stop1 - start1);
}


/// Calculate some statistics.
/// @param vals the data.
/// @param num_vals len vals.
/// @param res output stats.
void mathutils_CalcStats(double vals[], int num_vals, stat_results_t* res);


/// Calculate a crc32.
/// @param crc  starting value.
/// @param buf  calc values.
/// @param num_vals  size of buf.
/// @param If the calced crc ends up being 0, change this to 1.
///        This adds extremely small additional error but makes it handy in this application.
/// @return minty-fresh crc.
uint32_t mathutils_crc32(uint32_t crc, const uint8_t* buf, int num_vals, bool adjust_zero);


/// Calculate the mean.
/// @param vals the data.
/// @param num_vals len vals.
/// @return the mean or HUGE_VAL if invalid.
double mathutils_Mean(double vals[], int num_vals);


/// Calculate the sum of squares.
/// @param vals the data.
/// @param num_vals len vals.
/// @return the mean or HUGE_VAL if invalid.
double mathutils_SumofSquares(double vals[], int num_vals);


/// Calculate the sample std dev.
/// @param vals the data.
/// @param num_vals len vals.
/// @return the mean or HUGE_VAL if invalid.
double mathutils_StandardDeviation(double vals[], int num_vals);


#endif // MATHUTILS_H
