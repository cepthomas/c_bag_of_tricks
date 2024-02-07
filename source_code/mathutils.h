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
