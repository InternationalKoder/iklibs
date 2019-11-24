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

#ifndef IKLOG_MESSAGE_HPP
#define IKLOG_MESSAGE_HPP

#include <string>
#include <chrono>
#include "Level.hpp"
#include "iklog_export.hpp"

namespace iklog
{
    /*!
     * \brief Defines a logging message, only contains data
     */
    class Message
    {
        public:

            using Duration = std::chrono::steady_clock::duration;
            using TimePoint = std::chrono::system_clock::time_point;


            IKLOG_EXPORT Message(const std::string& logName, Level level, const std::string& message,
                                 const Duration& programDuration, const TimePoint& clockTime);

            IKLOG_EXPORT inline std::string getLogName() const { return m_logName; }
            IKLOG_EXPORT inline Level getLevel() const { return m_level; }
            IKLOG_EXPORT inline std::string getMessage() const { return m_message; }
            IKLOG_EXPORT inline Duration getProgramDuration() const { return m_programDuration; }
            IKLOG_EXPORT inline TimePoint getClockTime() const { return m_clockTime; }

        private:

            const std::string m_logName;
            const Level m_level;
            const std::string m_message;
            const Duration  m_programDuration;
            const TimePoint  m_clockTime;
    };
}

#endif // IKLOG_MESSAGE_HPP
