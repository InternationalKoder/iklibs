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
#include <iostream>

namespace iklog
{
    std::map<std::string, Log*> Log::m_logsList;
    OstreamWrapper Log::DEFAULT_OUTPUT(std::cout);

    Log::Log(const std::string& name, int levels, Output& output, const Formatter& formatter) :
        m_name(name),
        m_levels(levels),
        m_startTime(std::chrono::system_clock::now()),
        m_formatter(formatter)
    {
        m_logsList.insert_or_assign(name, this);

        m_outputs.insert_or_assign(Level::INFO,    &output);
        m_outputs.insert_or_assign(Level::DEBUG,   &output);
        m_outputs.insert_or_assign(Level::WARNING, &output);
        m_outputs.insert_or_assign(Level::ERROR,   &output);
    }

    void Log::log(Level level, const std::string& message) const
    {
        if(isLevelEnabled(level))
        {
            std::chrono::steady_clock::duration diff = std::chrono::system_clock::now() - m_startTime;

            Message logMessage(m_name, level, message, diff, std::chrono::system_clock::now());
            *m_outputs.at(level) << m_formatter.format(logMessage) << std::endl;
        }
    }
}
