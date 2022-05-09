#include <iostream>
#include "ascii-art/Menu.h"


int main(int argc, char * argv[]) {
    argv++;
    argc--;
    std::cout << "Loading sources.... " << std::endl;
    Menu menu(std::vector<std::string>(argv, argv + argc));
    std::cout << "Loading main menu.... " << std::endl;
    menu.start();
    return 0;
}
