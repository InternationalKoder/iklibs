#ifndef IKLOG_LOG_HPP
#define IKLOG_LOG_HPP

#include "Level.hpp"
#include "Formatter.hpp"
#include <map>
#include <chrono>
#include <ostream>
#include <thread>
#include <condition_variable>
#include <queue>
#include "iklog_export.hpp"

namespace iklog
{
    class Log
    {
        public:

            IKLOG_EXPORT Log(const std::string& name, unsigned int levels, const Formatter& formatter = Formatter());

            IKLOG_EXPORT void log(Level level, const std::string& message);

            IKLOG_EXPORT inline void info(const std::string& message) { log(Level::INFO, message); }
            IKLOG_EXPORT inline void debug(const std::string& message) { log(Level::DEBUG, message); }
            IKLOG_EXPORT inline void warn(const std::string& message) { log(Level::WARNING, message); }
            IKLOG_EXPORT inline void error(const std::string& message) { log(Level::ERROR, message); }

            IKLOG_EXPORT inline bool isLevelEnabled(Level level) const { return (m_levels & level) != 0; }
            IKLOG_EXPORT inline void enableLevels(unsigned int levels) { m_levels |= levels; }
            IKLOG_EXPORT inline void disableLevels(unsigned int levels) { m_levels &= ~levels; }

            IKLOG_EXPORT inline static Log* getLog(const std::string& name) { return m_logsList[name]; }

            IKLOG_EXPORT inline void setOutput(Level level, std::ostream& output) { m_outputs[level] = &output; }
            IKLOG_EXPORT inline void setFormatter(const Formatter& formatter) { m_formatter = formatter; }

        private:

            static std::map<std::string, Log*> m_logsList;


            const std::string m_name;
            std::map<Level, std::ostream*> m_outputs;
            unsigned int m_levels;
            std::chrono::system_clock::time_point m_startTime;
            Formatter m_formatter;
    };
}

#endif // IKLOG_LOG_HPP
