 
#include "test.h"
#include <iostream>


int main(int argc, char** argv) 
{
    std::cout << "test main here, argc: " << argc << std::endl;
    std::string filter;
    bool list = false;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--filter" && i + 1 < argc) filter = argv[++i];
        else if (a == "--list") list = true;
        else if (a == "-h" || a == "--help") {
            std::puts("Usage: c2j_tests [--filter Suite.Name|substring] [--list]");
            return 0;
        }
    }
    return ut::run_all(filter, list);
}
