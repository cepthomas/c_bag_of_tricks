#ifndef TIME_ANALYZER_H
#define TIME_ANALYZER_H


#include <windows.h>
#include <stdbool.h>


/// Number of data points to grab for statistics. TODO2 make user selectable.
#define NUM_SAMPLES 100

/// Analysis results in msec.
typedef struct time_results
{
    int num_samples;
    double mean;
    double min;
    double max;
    double sd;
} time_results_t;


/// Initialize.
/// @return status
bool timeanalyzer_Init(void); // TODO2 need unit tests.

/// Resets everything.
void timeanalyzer_Reset(void);

/// Start or stop the analysis.
/// @param[in] on T/F
/// @return running
bool timeanalyzer_Run(bool on);

/// Start timing capture.
void timeanalyzer_Arm(void);

/// Capture a time value.
/// @param[in] name desc
/// @return pointer to results if completed else NULL.
time_results_t* timeanalyzer_Grab(void);

#endif // TIME_ANALYZER_H
