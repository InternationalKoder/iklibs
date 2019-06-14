#include "iklog/Log.hpp"
#include <iostream>
#include "iklog/Message.hpp"

namespace iklog
{
    std::map<std::string, Log*> Log::m_logsList;

    Log::Log(const std::string& name, unsigned int levels, const Formatter& formatter) :
        m_name(name),
        m_levels(levels),
        m_startTime(std::chrono::system_clock::now()),
        m_formatter(formatter)
    {
        m_logsList[name] = this;

        m_outputs[Level::INFO] = &std::cout;
        m_outputs[Level::DEBUG] = &std::cout;
        m_outputs[Level::WARNING] = &std::cout;
        m_outputs[Level::ERROR] = &std::cerr;
    }

    void Log::log(Level level, const std::string& message)
    {
        if(isLevelEnabled(level))
        {
            std::chrono::steady_clock::duration diff = std::chrono::system_clock::now() - m_startTime;

            // possible to store log messages in memory for analysis
            Message logMessage(m_name, level, message, diff, std::chrono::system_clock::now());
            *m_outputs.at(level) << m_formatter.format(logMessage) << std::endl;
        }
    }
}
