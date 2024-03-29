
#ifndef FTIMER_H
#define FTIMER_H

#include <stdbool.h>


/// @brief A fast (msec) timer based on system multimedia timer. See READ_ME.

//---------------- Public API ----------------------//


/// Type: Function pointer for handling of timer ticks.
/// @param msec Since last time.
typedef void (*ftimer_InterruptFunc_t)(double msec);

/// Initialize the module.
/// @param fp Callback function on period.
/// @param ft_res Resolution of fast timer.
/// @return status
int ftimer_Init(ftimer_InterruptFunc_t fp, unsigned ft_res);

/// Enter the forever loop.
/// @param Desired period in msec or 0 to stop.
/// @return status
int ftimer_Run(double period);

/// Clean up module resources.
/// @return status
int ftimer_Destroy(void);

/// Current state.
/// @return T/F
bool ftimer_IsRunning(void);

#endif // FTIMER_H
