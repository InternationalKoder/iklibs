#include "Settings.hpp"
#include <iostream>
#include <iklog/Log.hpp>
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/readers/JsonReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>

int main()
{
    iklog::Log log("iklibs-examples", iklog::Level::DEBUG | iklog::Level::WARNING | iklog::Level::ERROR);
    log.info("Won't be displayed because the level is not activated");
    log.warn("This warning should be displayed");

    iklog::Log* examplesLog = iklog::Log::getLog("iklibs-examples");
    examplesLog->disableLevels(iklog::Level::DEBUG);
    log.debug("We retrieved a Log object and disabled debug level on it, this message should not appear");

    Settings settings;
    ikconf::PropertiesReader propertiesReader(settings);

    try
    {
        propertiesReader.read("resources/test.properties");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    std::cout << "Properties file:" << std::endl;
    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestCharAsNumber() << std::endl;


    ikconf::JsonReader jsonReader(settings);

    try
    {
        jsonReader.read("resources/test.json");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    std::cout << "JSON file:" << std::endl;
    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubString() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubInt() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubSubSettings().getTestSubSubFloat() << std::endl;
    std::cout << settings.getTestOtherSubSettings().getTestOtherSubSettingsStr() << std::endl;

    return 0;
}
