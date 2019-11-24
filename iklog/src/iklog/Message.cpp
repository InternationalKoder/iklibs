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

#include "iklog/Message.hpp"

namespace iklog
{
    Message::Message(const std::string& logName, Level level, const std::string& message,
                     const Duration& programDuration, const TimePoint& clockTime) :
        m_logName(logName),
        m_level(level),
        m_message(message),
        m_programDuration(programDuration),
        m_clockTime(clockTime)
    {}
}
