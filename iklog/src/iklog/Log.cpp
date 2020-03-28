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

#include "iklog/Log.hpp"
#include "iklog/Message.hpp"
#include "iklog/NullLog.hpp"
#include <iostream>

namespace iklog
{
    OstreamWrapper Log::DEFAULT_OUTPUT(std::cout);

    Log::Log(const std::string& name, int levels, Output& output, const Formatter& formatter) :
        m_name(name),
        m_levels(levels),
        m_startTime(std::chrono::system_clock::now()),
        m_formatter(formatter)
    {
        getLogsList()[name] = this;

        m_outputs[Level::INFO]    = &output;
        m_outputs[Level::DEBUG]   = &output;
        m_outputs[Level::WARNING] = &output;
        m_outputs[Level::ERROR]   = &output;
    }

    Log::~Log()
    {}

    void Log::log(Level level, const std::string& message) const
    {
        if(isLevelEnabled(level))
        {
            std::chrono::steady_clock::duration diff = std::chrono::system_clock::now() - m_startTime;

            Message logMessage(m_name, level, message, diff, std::chrono::system_clock::now());
            *m_outputs.at(level) << m_formatter.format(logMessage) << std::endl;
        }
    }

    Log* Log::getLog(const std::string& name)
    {
        auto it = getLogsList().find(name);
        if(it != getLogsList().end())
            return it->second;
        else
            return &NullLog::getInstance();
    }
}
