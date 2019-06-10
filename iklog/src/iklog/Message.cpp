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
    { }
}
