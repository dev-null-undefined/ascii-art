#include <iostream>
#include "ascii-art/Menu.h"


int main(int argc, char * argv[]) {
    argv++;
    argc--;
    Menu menu(std::vector<std::string>(argv, argv + argc));
    menu.start();
    return 0;
}
