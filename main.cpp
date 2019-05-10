#include <stdio.h>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    //TODO replace malloc/free for instrumenting test code.

    // Run the requested tests. Probably should be obtained from the command line.
    std::vector<std::string> whichSuites;
    // LIST SM SM_DOT SM_MAIN
    whichSuites.push_back("LIST");
    whichSuites.push_back("SM");
    tm.RunSuites(whichSuites, 'r');

    return 0;
}
