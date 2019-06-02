#include "Settings.hpp"
#include <iostream>

int main()
{
    Settings settings;

    settings.read();

    std::cout << settings.getResourcesPath() << std::endl;
    std::cout << settings.getThreadsNumber() << std::endl;

    return 0;
}
