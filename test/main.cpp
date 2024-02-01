#include <cstdio>
#include "pnut.h"

extern "C"
{
#include "logger.h"
}

int main()
{
    TestManager& tm = TestManager::Instance();

    // Run the requested tests. Could be obtained from the command line.
    std::vector<std::string> whichSuites;

    whichSuites.emplace_back("MATHUTILS");
    // whichSuites.emplace_back("LOGGER");
    // whichSuites.emplace_back("SM");
    // whichSuites.emplace_back("DIAG");
    // whichSuites.emplace_back("LIST");
    // whichSuites.emplace_back("STR");
    // whichSuites.emplace_back("DICT");


    whichSuites.emplace_back("STOPWATCH");
    whichSuites.emplace_back("FTIMER");
    // whichSuites.emplace_back("TMAN");

    // Init system before running tests.
    FILE* fp = fopen("cbot_log.txt", "a");
    logger_Init(fp);
    // logger_Init(stdout);
    logger_SetFilters(LVL_DEBUG);
    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml
    fclose(fp);

    return 0;
}
