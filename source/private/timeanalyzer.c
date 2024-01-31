
#include <windows.h>
#include <math.h>

#include "cbot.h"
#include "timeanalyzer.h"

// TODO1 implement/test. Consolidate with stopwatch?

//---------------- Privates ------------------------------//

#define MAX_SAMPLES 100

/// How many.
static int _num_samples = 0;

// State.
static bool _running = false;

// State.
static bool _armed = false;

/// Periodic time.
static long long _last_tick = -1;

/// False if high res is not available.
static bool _valid = false;

/// The current performance counter scale, in ticks per msec.
static double _ticks_per_msec;

/// Delay at start.
static int _skip_count = 0;

/// Number of initial data points to exclude from stats.
//static int _skip = 0;

/// Accumulated data points.
static double _samples[MAX_SAMPLES];

/// Location in _samples.
static int _sample_index = 0;

/// Analysis results.
static time_results_t _results;


//---------------- Privates ----------------------------//

// The current performance counter value.
// @return ticks
static long long _CurrentTick(void)
{
    LARGE_INTEGER ts;
    QueryPerformanceCounter(&ts);
    return ts.QuadPart;
}

// Conversion for values.
// @return msec
static double _TicksToMsec(long long ticks)
{
    return 1000.0 * ticks / _ticks_per_msec;
}


//---------------- Public API ----------------------------//

//--------------------------------------------------------//
int timeanalyzer_Init(int num_samples)
{
    int stat = CBOT_ERR_NO_ERR;
    if (num_samples > MAX_SAMPLES)
    {
        stat = EINVAL;
        _valid = false;
    }
    else
    {
        _num_samples = num_samples;
        _valid = true;
    }

    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    _ticks_per_msec = (double)f.QuadPart / 1000.0;
    _last_tick = 0;
    timeanalyzer_Reset();

    return stat;
}


//--------------------------------------------------------//
void timeanalyzer_Reset(void)
{
    _running = false;
    _armed = false;

    // Clear current.
    memset(&_results, 0, sizeof(_results));
    memset(_samples, 0, sizeof(_samples));
    _sample_index = 0;
}


//--------------------------------------------------------//
bool timeanalyzer_Run(bool on)
{
    if(_valid)
    {
        _running = on;
    }

    return _running;
}


//--------------------------------------------------------//
void timeanalyzer_Arm()
{
    _last_tick = _CurrentTick(); // snap!
    _armed = true;
}


//--------------------------------------------------------//
time_results_t* timeanalyzer_Grab()
{
    if (!_armed)
    {
        return NULL;
    }

    bool stats = false;

    long long et = _CurrentTick(); // snap!

    if (_last_tick != -1 && _skip_count-- < 0)
    {
        double dt = _TicksToMsec(et - _last_tick);
        _samples[_sample_index++] = dt;
    }
    _last_tick = et;

    if (_sample_index >= _num_samples)
    {
        // Process the collected stuff.
        _results.mean = 0;
        _results.min = HUGE_VAL;
        _results.max = -HUGE_VAL;
        _results.sd = 0;

        for (int i = 0; i < _num_samples; i++)
        {
            _results.mean += _samples[i];
            _results.min = _samples[i] < _results.min ? _samples[i] : _results.min;
            _results.max = _samples[i] > _results.max ? _samples[i] : _results.max;
        }

        // Mean.
        _results.mean /= _num_samples;

        // Std dev.
        double sum_of_squares = 0;
        for (int i = 0; i < _num_samples; i++)
        {
            sum_of_squares += pow((_samples[i] - _results.mean), 2);
        }

        _results.sd = sum_of_squares / (_num_samples - 1);
        _results.sd = sqrt(_results.sd);

        stats = true;
        timeanalyzer_Reset();
    }

    return stats ? &_results : NULL;
}
