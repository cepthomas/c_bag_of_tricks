

#include <windows.h>

#include "stopwatch.h"
#include "ftimer.h"

//---------------- Private --------------------------//

/// Registered client callback.
static ftimer_InterruptFunc_t p_interrupt_func = NULL;

static unsigned p_period = 0;

static MMRESULT p_htimer = -1;

static void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

static int _counter = 0;


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int ftimer_Init(ftimer_InterruptFunc_t fp)
{
    int stat = 0;

    p_interrupt_func = fp;

    // if (!Stopwatch.IsHighResolution)
    // {
    //     throw new Exception("High res performance counter is not available.");
    // }

    // // Initialize timer with default values.
    // _timeProc = new TimeProc(MmTimerCallback);

    return stat;
}

//--------------------------------------------------------//
int ftimer_Run(unsigned period)
{
    int stat = 0;

    p_period = period;

    if(period > 0)
    {
        // Clean up if already running.
        if(p_htimer >= 0)
        {
            timeKillEvent(p_htimer);
        }

        /// Timer resolution in milliseconds. The resolution increases with smaller values - a resolution of 0
        /// indicates periodic events should occur with the greatest possible accuracy. To reduce system 
        /// overhead, however, you should use the maximum value appropriate for your application.

        p_htimer = timeSetEvent(10, 0, p_TimerCallback, 0, TIME_PERIODIC);
    }
    else
    {
        if(p_htimer >= 0)
        {
            timeKillEvent(p_htimer);
        }
        p_htimer = -1;

        //check return result != TIMERR_NOERROR
    }

    return stat;
}

//--------------------------------------------------------//
int ftimer_Destroy(void)
{
    int stat = 0;

    if(p_htimer >= 0)
    {
        timeKillEvent(p_htimer);
    }

    p_htimer = -1;
    p_period = 0;
    p_interrupt_func = NULL;

    return stat;
}


//--------------------------------------------------------//
long ftimer_CurrentUsec(void)
{
    return 9999;
}


//---------------- Private Implementation -------------//

//--------------------------------------------------------//
void CALLBACK p_TimerCallback(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    _counter++;



    /// <summary>Used for more accurate timing.</summary>
    // readonly Stopwatch _sw = new Stopwatch();

    // /// <summary>Indicates whether or not the timer is running.</summary>
    // bool _running = false;

    // /// <summary>Stopwatch support.</summary>
    // long _lastTicks = -1;

    // if (_running)
    // {
    //     if(_lastTicks != -1)
    //     {
    //         // When are we?
    //         long t = _sw.ElapsedTicks; // snap
    //         double msec = (t - _lastTicks) * 1000D / Stopwatch.Frequency;
    //         _lastTicks = t;

    //         // Check for expirations.
    //         timer.current += msec;
    //         if ((timer.period - timer.current) < allowance)
    //         {
    //             elapsed.Add(tid);
    //             timer.current = 0.0;
    //             TimerElapsedEvent?.Invoke(this, new TimerEventArgs() { ElapsedTimers = elapsed });
    //         }
    //     }
    //     else
    //     {
    //         // Starting.
    //         _lastTicks = _sw.ElapsedTicks;
    //     }
    // }

}
