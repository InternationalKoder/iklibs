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
    /*!
     * \brief Allows to log a message
     */
    class Log
    {
        public:

            /*!
             * \brief Constructor
             * \param name The name of the Log
             * \param levels List of the enabled logging levels (flags)
             * \param formatter The formatter to use for the logging messages
             */
            IKLOG_EXPORT Log(const std::string& name, unsigned int levels, const Formatter& formatter = Formatter());


            /*!
             * \brief Logs a message in the given level
             * \param level The level of the logging message
             * \param message The message to log
             */
            IKLOG_EXPORT void log(Level level, const std::string& message);


            /*!
             * \brief Logs a message in INFO level
             * \param message The message to log
             */
            IKLOG_EXPORT inline void info(const std::string& message) { log(Level::INFO, message); }

            /*!
             * \brief Logs a message in DEBUG level
             * \param message The message to log
             */
            IKLOG_EXPORT inline void debug(const std::string& message) { log(Level::DEBUG, message); }

            /*!
             * \brief Logs a message in WARNING level
             * \param message The message to log
             */
            IKLOG_EXPORT inline void warn(const std::string& message) { log(Level::WARNING, message); }

            /*!
             * \brief Logs a message in ERROR level
             * \param message The message to log
             */
            IKLOG_EXPORT inline void error(const std::string& message) { log(Level::ERROR, message); }


            /*!
             * \brief Checks whether a level is enabled for this Log
             * \param level The level to check
             * \return True if the level is enabled
             */
            IKLOG_EXPORT inline bool isLevelEnabled(Level level) const { return (m_levels & level) != 0; }


            /*!
             * \brief Enables the given levels
             * \param levels List of flags describing the levels to enable
             */
            IKLOG_EXPORT inline void enableLevels(unsigned int levels) { m_levels |= levels; }

            /*!
             * \brief Disables the given levels
             * \param levels List of flags describing the levels to disable
             */
            IKLOG_EXPORT inline void disableLevels(unsigned int levels) { m_levels &= ~levels; }


            /*!
             * \brief Gives the Log object having a specific name
             * \param name The name of the Log to fetch
             * \return The Log with the given name if it has been found
             */
            IKLOG_EXPORT inline static Log* getLog(const std::string& name) { return m_logsList[name]; }


            /*!
             * \brief Changes the output stream for a specific level
             * \param level The level that will have a new output stream
             * \param output The output stream to use
             */
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
