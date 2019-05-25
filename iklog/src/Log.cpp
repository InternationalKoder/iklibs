#include "Log.hpp"
#include <iostream>
#include "Message.hpp"

namespace iklog
{
    Log::Log(unsigned int levels, const Formatter& formatter) :
        m_levels(levels),
        m_startTime(std::chrono::system_clock::now()),
        m_formatter(formatter),
        m_workerThread(&Log::worker, this),
        m_stop(false)
    {
        m_outputs[Level::INFO] = &std::cout;
        m_outputs[Level::DEBUG] = &std::cout;
        m_outputs[Level::WARNING] = &std::cout;
        m_outputs[Level::ERROR] = &std::cerr;
    }

    Log::~Log()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
        m_conditionVar.notify_all();
        lock.unlock();
        m_workerThread.join();
    }

    void Log::log(Level level, const std::string& message)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::pair<Level, std::string>(level, message));
        lock.unlock();
        m_conditionVar.notify_one();
    }

    void Log::worker()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_conditionVar.wait(lock, [this]() { return m_stop || !m_queue.empty(); });

            if(!m_queue.empty())
            {
                std::pair<Level, std::string> elem = m_queue.front();
                m_queue.pop();

                const Level& level = elem.first;
                const std::string& message = elem.second;

                if(isLevelEnabled(level))
                {
                    std::chrono::steady_clock::duration diff = std::chrono::system_clock::now() - m_startTime;

                    // possible to store log messages in memory for analysis
                    Message logMessage(level, message, diff, std::chrono::system_clock::now());
                    *m_outputs.at(level) << m_formatter.format(logMessage) << std::endl;
                }
            }
            else if(m_stop)
            {
                break;
            }
        }
    }
}
