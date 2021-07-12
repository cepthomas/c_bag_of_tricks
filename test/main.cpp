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

    whichSuites.emplace_back("LOGGER");

    whichSuites.emplace_back("SM");
    whichSuites.emplace_back("COMMON");
    whichSuites.emplace_back("DICT");
    whichSuites.emplace_back("STR");
    whichSuites.emplace_back("LIST");

    // Init system before running tests.
    common_Init();
    FILE* fp = fopen("log_test.txt", "w");
    logger_Init(fp);

    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml

    return 0;
}
