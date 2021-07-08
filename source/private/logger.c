
#include <stdio.h>
#include <string.h>
#include "logger.h"


/// @brief Definition of dictionary thing.


//---------------- Private Declarations ------------------//

log_level_t p_level = LOG_INFO;
int p_cat = 0xFFFFFFFF;
FILE* p_fp = NULL;


//---------------- Public API Implementation -------------//

//--------------------------------------------------------//
int logger_Init(FILE* fp)
{
    p_fp = fp == NULL ? stdout : fp;
    return 0;
}

//--------------------------------------------------------//
int logger_SetFilters(log_level_t level, int cat)
{
    p_level = level;
    p_cat = cat;
    return 0;
}

//--------------------------------------------------------//
int logger_Destroy(void)
{
    // Anything?
    return 0;
}


// _trace_cat = TraceCat.ERROR | TraceCat.LOOK | TraceCat.INFO | TraceCat.STDO # | TraceCat.LOAD | TraceCat.ACTV


//--------------------------------------------------------//
void logger_Log(log_level_t level, int cats, int line, const char* format, ...)
{


    // VAL_PTR(sm, RS_ERR);
    // VAL_PTR(format, RS_ERR);

    // int ret = RS_PASS;

    // if(sm->fp != NULL)
    // {
    //     va_list args;
    //     va_start(args, format);

    //     char sfmt[MAX_TRACE];
    //     vsnprintf(sfmt, MAX_TRACE-1, format, args);
    //     va_end(args);

    //     fprintf(sm->fp, "SM(%d): %s", line, sfmt);
    // }




//status_t common_log(loglvl_t level, const char* format, ...)
// {
//     status_t stat = STATUS_OK;
  
//     static char buff[LOG_LINE_LEN];

//     sprintf(buff, "%s ", common_xlatLogLevel(level));
//     va_list args;
//     va_start(args, format);
//     vsnprintf(buff + strlen(buff), LOG_LINE_LEN - 1, format, args);
//     // board_log(buff); >>>     board_serWriteLine(txt); >>> printf()


//     return stat;
// }






    // global _trace_fn, _temp_path, _store_path

    // if _trace_fn is None:
    //     # First time = initialize. Make sure paths exist.
    //     _temp_path = os.path.join(sublime.packages_path(), 'SublimeBagOfTricks', 'temp')
    //     pathlib.Path(_temp_path).mkdir(parents=True, exist_ok=True)
    //     _store_path = os.path.join(sublime.packages_path(), 'SublimeBagOfTricks', 'store')
    //     pathlib.Path(_store_path).mkdir(parents=True, exist_ok=True)
    //     # The trace file.
    //     _trace_fn = os.path.join(_temp_path, 'trace.txt')

    // if cat in _trace_cat:
    //     now = datetime.datetime.now().time()

    //     scat = str(cat).replace('TraceCat.', '')
    //     if cat == TraceCat.ERROR:
    //         scat = '!!!!!!!! ERROR !!!!!!!!!!!!'
    //     elif cat == TraceCat.LOOK:
    //         scat = '>>>>>>>>> LOOK >>>>>>>>>>>>'

    //     content = ' | '.join(map(str, args))
    //     s = f'{now} {scat} {content}'

    //     with open(_trace_fn, "a+") as f:
    //         f.write(s + '\n')

    //     # Check for file size limit.
    //     if os.path.getsize(_trace_fn) > 100000:
    //         try:
    //             os.replace(_trace_fn, _trace_fn.replace('trace.txt', 'trace_old.txt'))
    //             os.remove(_trace_fn)
    //         except Exception as e:
    //             pass


}


//---------------- Private Implementation --------------------------//

//--------------------------------------------------------//
const char* p_XlatLogLevel(log_level_t level)
{
    static char buff[20];
    switch(level)
    {
        case LOG_INFO:  strcpy(buff, "INF"); break;
        case LOG_WARN:  strcpy(buff, "WRN"); break;
        case LOG_ERROR: strcpy(buff, "ERR"); break;
        default: snprintf(buff, 20, "%d", level); break;
    }
    return buff;
}    

