/*
    Copyright (C) 2019, InternationalKoder

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

#include "iklog/Formatter.hpp"
#include <ctime>
#include <stdexcept>
#include <cassert>
#include <iostream>

namespace iklog
{
    typedef std::string(*getFunc)(const Message&);


    const std::map<char, getFunc> Formatter::FORMAT_MAPPING
    {
        {'L', &Formatter::getLogName},
        {'l', &Formatter::getLevel},
        {'p', &Formatter::getLevelPretty},
        {'m', &Formatter::getMessage},
        {'d', &Formatter::getProgramDuration},
        {'t', &Formatter::getClockTime}
    };


    Formatter::Formatter(const std::string& format) :
        m_format(format)
    {}


    std::string Formatter::format(const Message& message) const
    {
        std::string formatted(m_format);

        for(std::string::size_type i = 0 ; i < formatted.size() - 1 ; i++)
        {
            if(formatted[i] == '%')
            {
                formatted.replace(i, 2, (FORMAT_MAPPING.at(formatted[i+1]))(message));
            }
        }

        return formatted;
    }


    std::string Formatter::getLevel(const Message& message)
    {
        const Level& level = message.getLevel();
        assert(level == Level::INFO || level == Level::DEBUG || level == Level::WARNING || level == Level::ERROR);

        switch(level)
        {
            case Level::INFO:
                return "INFO";
            case Level::DEBUG:
                return "DEBUG";
            case Level::WARNING:
                return "WARNING";
            case Level::ERROR:
                return "ERROR";
        }

        return "";
    }


    std::string Formatter::getLevelPretty(const Message& message)
    {
        const Level& level = message.getLevel();
        assert(level == Level::INFO || level == Level::DEBUG || level == Level::WARNING || level == Level::ERROR);

        switch(level)
        {
            case Level::INFO:
                return "INFO";
            case Level::DEBUG:
                return "DBUG";
            case Level::WARNING:
                return "WARN";
            case Level::ERROR:
                return "ERR ";
        }

        return "    ";
    }


    std::string Formatter::getProgramDuration(const Message& message)
    {
        const Message::Duration& DURATION = message.getProgramDuration();

        const auto HOURS = std::chrono::duration_cast<std::chrono::hours>(DURATION).count();
        const auto MINUTES = std::chrono::duration_cast<std::chrono::minutes>(DURATION).count() % 60;
        const auto SECONDS = std::chrono::duration_cast<std::chrono::seconds>(DURATION).count() % 60;

        std::string duration = std::to_string(HOURS) + ":";
        if(MINUTES < 10)
            duration += "0";
        duration += std::to_string(MINUTES) + ":";
        if(SECONDS < 10)
            duration += "0";
        duration += std::to_string(SECONDS);

        return duration;
    }


    std::string Formatter::getClockTime(const Message& message)
    {
        const Message::TimePoint& TIME = message.getClockTime();
        std::time_t time = std::chrono::system_clock::to_time_t(TIME);

        struct tm tm;
        #ifdef _WIN32
            localtime_s(&tm, &time);
        #elif defined __unix__
            localtime_r(&time, &tm);
        #endif

        char buff[32];
        strftime(buff, 32, "%a, %Y-%m-%d %H:%M:%S", &tm);

        return buff;
    }
}
