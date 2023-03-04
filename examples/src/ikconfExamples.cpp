/*
    Copyright (C) 2023, InternationalKoder

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

#include "ikconfExamples.hpp"

#include "ikconf/Settings.hpp"
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/readers/JsonReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>
#include <iostream>

void runIkconfExamples()
{
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
}
