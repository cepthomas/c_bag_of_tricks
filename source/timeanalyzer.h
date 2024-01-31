#ifndef TIME_ANALYZER_H
#define TIME_ANALYZER_H

#include <windows.h>
#include <stdbool.h>


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
/// @param num_samples how many
/// @return status
int timeanalyzer_Init(int max_samples);

/// Resets everything.
void timeanalyzer_Reset(void);

// /// Start or stop the analysis.
// /// @param on T/F
// /// @return running
// bool timeanalyzer_Run(bool on);

/// Start timing capture.
void timeanalyzer_Arm(void);

/// Capture a time value.
/// @param name desc
/// @return pointer to results if completed else NULL.
time_results_t* timeanalyzer_Grab(void);

#endif // TIME_ANALYZER_H
