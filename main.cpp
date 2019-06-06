#include <cstdio>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    // Run the requested tests. Probably should be obtained from the command line.
    std::vector<std::string> whichSuites;
    // LIST SM SM_DOT SM_MAIN
    whichSuites.emplace_back("LIST");
    whichSuites.emplace_back("SM");
    tm.RunSuites(whichSuites, 'r');

    return 0;
}
