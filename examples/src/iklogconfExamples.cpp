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

#include "iklogconfExamples.hpp"

#include <iklogconf/LogConfigurator.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>

void runIklogconfExamples()
{
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
}
