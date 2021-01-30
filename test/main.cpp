#include <cstdio>
#include "pnut.h"

int main()
{
    TestManager& tm = TestManager::Instance();

    // Run the requested tests. Could be obtained from the command line.
    std::vector<std::string> whichSuites;

    // printf("aaaaa\n");

    whichSuites.emplace_back("DICT");
    whichSuites.emplace_back("STR");
    whichSuites.emplace_back("LIST");
    whichSuites.emplace_back("SM");
    // whichSuites.emplace_back("PNUT");

    // printf("bbbbb\n");

    tm.RunSuites(whichSuites, 'r'); // 'r' for readable, 'x' for xml

    // printf("ccccc\n");

    return 0;
}
