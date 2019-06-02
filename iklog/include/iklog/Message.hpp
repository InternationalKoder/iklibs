#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <chrono>
#include "Level.hpp"
#include "iklog_export.hpp"

namespace iklog
{
    class Message
    {
        public:

            using Duration = std::chrono::steady_clock::duration;
            using TimePoint = std::chrono::system_clock::time_point;

            IKLOG_EXPORT Message(Level level, const std::string& message, const Duration& programDuration, const TimePoint& clockTime);

            IKLOG_EXPORT inline Level getLevel() const { return m_level; }
            IKLOG_EXPORT inline std::string getMessage() const { return m_message; }
            IKLOG_EXPORT inline Duration getProgramDuration() const { return m_programDuration; }
            IKLOG_EXPORT inline TimePoint getClockTime() const { return m_clockTime; }

        private:

            Level m_level;
            std::string m_message;
            Duration  m_programDuration;
            TimePoint  m_clockTime;
    };
}

#endif
