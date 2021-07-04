
#ifndef STOPWATCH_H
#define STOPWATCH_H


#include <stdbool.h>

//#include "common.h"


/// @brief Stopwatch similar to C# class.

//---------------- Public API ----------------------//


//const int SW_OK = 0;


bool stopwatch_IsRunning(void);

int stopwatch_Init(void);

int stopwatch_Start(void);

int stopwatch_Stop(void);

long stopwatch_ElapsedMilliseconds(void);

long stopwatch_ElapsedTicks(void);



// /// Type: Function pointer for handling of timer ticks.
// typedef void (*fpTimerInterrupt)(void);

// /// Initialize the module.
// /// @param[in] pint
// /// @return Status.
// status_t ftimer_init(fpTimerInterrupt fp);

// /// Enter the forever loop.
// /// @param[in] period in msec, 0 stops.
// /// @return Status on exit.
// status_t ftimer_run(uint period);

// /// Clean up module resources.
// /// @return Status.
// status_t ftimer_destroy(void);

// /// Get number of microseconds since beginning.
// /// @return Microseconds.
// uint64_t ftimer_getCurrentUsec(void);

#endif // STOPWATCH_H
