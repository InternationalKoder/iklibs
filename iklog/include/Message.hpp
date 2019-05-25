#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <chrono>
#include "Level.hpp"

namespace iklog
{
    class Message
    {
        public:

            typedef std::chrono::steady_clock::duration Duration;
            typedef std::chrono::system_clock::time_point TimePoint;

            Message(Level level, const std::string& message, const Duration& programDuration, const TimePoint& clockTime);

            inline Level getLevel() const { return m_level; }
            inline std::string getMessage() const { return m_message; }
            inline Duration getProgramDuration() const { return m_programDuration; }
            inline TimePoint getClockTime() const { return m_clockTime; }

        private:

            Level m_level;
            std::string m_message;
            Duration  m_programDuration;
            TimePoint  m_clockTime;
    };
}

#endif
