#include <cstdio>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    // Run the requested tests. Could be obtained from the command line.
    std::vector<std::string> whichSuites;

    whichSuites.emplace_back("STR");
    whichSuites.emplace_back("LIST");
    whichSuites.emplace_back("SM");
    // whichSuites.emplace_back("PNUT");

    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml

    //TODO printf("Don't forget to run_mem_chk!\n");

    return 0;
}
