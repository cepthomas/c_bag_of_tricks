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

    whichSuites.emplace_back("CHKSUM");
    whichSuites.emplace_back("LOGGER");
    whichSuites.emplace_back("SM");
    whichSuites.emplace_back("DIAG");
    whichSuites.emplace_back("LIST");
    whichSuites.emplace_back("STR");
    whichSuites.emplace_back("DICT");

    // Init system before running tests.
    FILE* fp = fopen("log_test_out.txt", "w");
    logger_Init(fp);
    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml

    return 0;
}
