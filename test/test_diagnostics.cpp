#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "pnut.h"

extern "C"
{
#include "diagnostics.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(DIAG_MEM, "Test memory probe functions. Make sure PROBE is set in diagnostics.h.")
{
 //   logger_SetFilters(LVL_DEBUG, CAT_MEM);

    const int STR_LEN = 16;
    const int NUM_TS = 20;

    typedef struct
    {
        int anumber;
        char* astring;
    } test_struct_t;

    // Allocate some heap.
    test_struct_t* test_structs[NUM_TS];

    for(int i = 0; i < NUM_TS; i++)
    {
        CREATE_INST(ts, test_struct_t);
        CREATE_STR(s, STR_LEN);
        ts->anumber = 11 * (i + 1);
        ts->astring = s;
        test_structs[i] = ts;
    }

    // Deallocate but miss one.
    for(int i = 0; i < NUM_TS; i++)
    {
        if(i != 7)
        {
            test_struct_t* p = test_structs[i];
            FREE(p->astring);
            FREE(p);
        }
    }

    return 0;
}    
