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

#include "iklogExamples.hpp"

#include <iklog/Log.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>

using namespace iklog::literals;

void runIklogExamples()
{
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
}
