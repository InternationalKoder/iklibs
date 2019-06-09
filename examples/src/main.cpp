#include "Settings.hpp"
#include <iostream>
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>

int main()
{
    Settings settings;
    ikconf::PropertiesReader reader(settings);

    try
    {
        reader.read("resources/test.properties");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestCharAsNumber() << std::endl;

    return 0;
}
