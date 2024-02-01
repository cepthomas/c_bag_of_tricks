
#include <Windows.h>
#include <cstdio>
#include <cstring>

#include "pnut.h"
#include "cbot.h"

extern "C"
{
// #include "stopwatch.h"
#include "timeanalyzer.h"
}



/////////////////////////////////////////////////////////////////////////////
UT_SUITE(TMAN_BASIC, "Test all timeanalyzer functions.")
{
    int stat = timeanalyzer_Init(55);
    UT_EQUAL(stat, CBOT_ERR_NO_ERR);


    // Start timing capture.
    // void timeanalyzer_Arm(void);


    // Capture a time value. return pointer to results if completed else NULL.
    // time_results_t* timeanalyzer_Grab(void);


    // void timeanalyzer_Reset(void);


    return 0;
}


/*        
    /// <summary>Fast timer.</summary>
    MmTimerEx _timer = new();

    /// <summary>Measurer.</summary>
    readonly TimingAnalyzer _tan = new();

    /// <summary>State.</summary>
    bool _running = false;

    /// <summary>How many.</summary>
    const int NUM_SAMPLES = 1000;

    public override void RunSuite()
    {
        UT_INFO("Tests MmTimerEx functions.");

        // Fast mm timer under test.
        _timer = new MmTimerEx();

        _timer.SetTimer(10, TimerElapsedEvent);

        _running = true;
        _tan.SampleSize = NUM_SAMPLES;
        _tan.Grab(); // this starts the tan.
        _timer.Start();

        // Do some background work.
        //for(int i = 0; _running; i++)
        //{
        //    for(int j = 0; j < 100; j++)
        //    {
        //        string s = $"abcdef {i} {j}";
        //        //Console.WriteLine(s);
        //    }
        //}
        while (_running) ;

        // Done - dump what we found.
        List<string> ls = new() { $"Msec" };

        // Time ordered.
        _tan.Times.ForEach(t => ls.Add($"{t}"));
        File.WriteAllLines(@"..\..\out\intervals_ordered.csv", ls);

        // Sorted by (rounded) times.
        Dictionary<double, int> _bins = new();
        for (int i = 0; i < _tan.Times.Count; i++)
        {
            var t = Math.Round(_tan.Times[i], 2);
            _bins[t] = _bins.ContainsKey(t) ? _bins[t] + 1 : 1;
        }
        ls.Clear();
        ls.Add($"Msec,Count");
        var vv = _bins.Keys.ToList();
        vv.Sort();
        vv.ForEach(v => ls.Add($"{v},{_bins[v]}"));
        File.WriteAllLines(@"..\..\out\intervals_sorted.csv", ls);

        //Debug.WriteLine(_tan);
        //Count:1000 Mean:9.982 Max:10.977 Min:9.040 SD:0.096

        // Resource clean up.
        _timer.Stop();
        _timer.Dispose();
    }

    /// <summary>
    /// Multimedia timer tick handler.
    /// </summary>
    void TimerElapsedEvent(double totalElapsed, double periodElapsed)
    {
        if (_tan.Grab())
        {
            // All done.
            _running = false;
            _timer.Stop();
        }
    }
    */
