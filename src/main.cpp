#include <iostream>
#include <variant>
#include "ascii-art/Application.h"

int main(int argc, char * argv[]) {
    argv++;
    argc--;
    std::cout << "Loading sources.... " << std::endl;
    Application application(std::vector<std::string>(argv, argv + argc));
    std::cout << "Loading main menu.... " << std::endl;
    application.start();
    return 0;
}
