/*
    Copyright (C) 2019, 2020, InternationalKoder

    This file is part of IKLibs.

    IKLibs is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IKLibs is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with IKLibs.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Settings.hpp"
#include <iostream>
#include <iklog/Log.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/readers/JsonReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>
#include <ikparll/ThreadPool.hpp>
#include <iklogconf/LogConfigurator.hpp>

using namespace iklog::literals;

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

    iklog::RollingFileOutput rollingFileOutput("examples.log", 512_b, 5);
    iklog::Log rollingFileLog("rolling-file", iklog::Level::INFO, rollingFileOutput);

    for(unsigned int i = 0 ; i < 80 ; i++)
        rollingFileLog.info("Line number " + std::to_string(i));

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

    for(const SubSettingsArrayItem& i : settings.getTestSubSettingsArray())
    {
        std::cout << "object array item: { " << i.getIndex() << " ; " << i.getValue() << " }" << std::endl;
    }

    // -------- IKPARLL
    std::cout << "Thread pool test" << std::endl;
    ikparll::ThreadPool<int, 3> threadPool([](int i) { std::cout << std::this_thread::get_id() << " : " << i*2 << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });
    for(int i = 0 ; i < 10 ; i++)
    {
        threadPool.addItem(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // -------- IKLOGCONF
    iklogconf::LogConfigurator logConfigurator;
    try
    {
        logConfigurator.readJsonFile("resources/log.json");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting log configuration: " << e.what() << std::endl;
    }

    iklog::Log* const iklogconfStdoutLogger = iklog::Log::getLog("iklogconfStdoutLogger");
    iklogconfStdoutLogger->info("This log has been configured from a file!");
    iklogconfStdoutLogger->debug("This message should not be displayed");

    iklog::Log* const iklogconfRollingFileLogger = iklog::Log::getLog("iklogconfRollingFileLogger");
    for(unsigned int i = 0 ; i < 10 ; i++)
    {
        iklogconfRollingFileLogger->info("Message " + std::to_string(i) + " on log configured from file");
    }

    iklog::Log* const iklogconfSingleOutput1 = iklog::Log::getLog("iklogconfSingleOutput-1");
    iklog::Log* const iklogconfSingleOutput2 = iklog::Log::getLog("iklogconfSingleOutput-2");

    iklogconfSingleOutput1->error("this log uses");
    iklogconfSingleOutput2->warn("the same output as this one");
    iklogconfSingleOutput1->error("and it works very well");


    return EXIT_SUCCESS;
}
