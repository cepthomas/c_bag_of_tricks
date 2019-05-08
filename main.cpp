#include <stdio.h>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    //TODO replace malloc/free for instrumenting test code.

    // Run the requested tests. Probably should be obtained from the command line.
    std::vector<std::string> whichSuites;
    whichSuites.push_back("PNUT"); // LIST SM PNUT
    tm.RunSuites(whichSuites, 'r');

    return 0;
}
