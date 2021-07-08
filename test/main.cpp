#include <cstdio>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    // Run the requested tests. Could be obtained from the command line.
    std::vector<std::string> whichSuites;

    whichSuites.emplace_back("COMMON");
    whichSuites.emplace_back("LOGGER");
    whichSuites.emplace_back("DICT");
    whichSuites.emplace_back("STR");
    whichSuites.emplace_back("LIST");
    whichSuites.emplace_back("SM");

    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml

    return 0;
}
