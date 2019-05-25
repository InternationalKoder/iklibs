#ifndef LOG_HPP
#define LOG_HPP

#include "Level.hpp"
#include "Formatter.hpp"
#include <map>
#include <chrono>
#include <ostream>
#include <thread>
#include <condition_variable>
#include <queue>

namespace iklog
{
    class Log
    {
        public:

            Log(unsigned int levels, const Formatter& formatter = Formatter());

            ~Log();

            void log(Level level, const std::string& message);

            inline void info(const std::string& message) { log(Level::INFO, message); }
            inline void debug(const std::string& message) { log(Level::DEBUG, message); }
            inline void warn(const std::string& message) { log(Level::WARNING, message); }
            inline void error(const std::string& message) { log(Level::ERROR, message); }

            inline bool isLevelEnabled(Level level) const { return (m_levels & level) != 0; }

            inline void setOutput(Level level, std::ostream& output) { m_outputs[level] = &output; }
            inline void setFormatter(const Formatter& formatter) { m_formatter = formatter; }

        private:

            void worker();

            std::map<Level, std::ostream*> m_outputs;
            unsigned int m_levels;
            std::chrono::system_clock::time_point m_startTime;
            Formatter m_formatter;
            std::queue<std::pair<Level, std::string>> m_queue;

            std::thread m_workerThread;
            std::condition_variable m_conditionVar;
            std::mutex m_mutex;
            bool m_stop;
    };
}

#endif
