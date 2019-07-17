#include "Settings.hpp"
#include <iostream>
#include <iklog/Log.hpp>
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/readers/JsonReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>

int main()
{
    // -------- IKLOG
    // create a logger with a name and enabled logging levels, then try to log some messages in different levels
    iklog::Log log("iklibs-examples", iklog::Level::DEBUG | iklog::Level::WARNING | iklog::Level::ERROR);
    log.info("Won't be displayed because the level is not activated");
    log.warn("This warning should be displayed");

    // retrieve a logger from its name
    iklog::Log* examplesLog = iklog::Log::getLog("iklibs-examples");
    examplesLog->disableLevels(iklog::Level::DEBUG);
    log.debug("We retrieved a Log object and disabled debug level on it, this message should not appear");

    // -------- IKCONF
    // create the object that will hold our configuration
    Settings settings;

    // read from a .properties file
    ikconf::PropertiesReader propertiesReader(settings);

    try
    {
        propertiesReader.read("resources/test.properties");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    // display the read values from the file
    std::cout << "Properties file:" << std::endl;
    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestCharAsNumber() << std::endl;


    // now read from a .json file
    ikconf::JsonReader jsonReader(settings);

    try
    {
        jsonReader.read("resources/test.json");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    // display the read values from the file
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

    for(const int& i : settings.getTestArray())
    {
        std::cout << "array item: " << i << std::endl;
    }

    for(const std::string& i : settings.getTestStringArray())
    {
        std::cout << "string array item: " << i << std::endl;
    }

    return EXIT_SUCCESS;
}
