/*
    Copyright (C) 2019, 2020, 2023, InternationalKoder

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

#ifndef IKLOG_LOG_HPP
#define IKLOG_LOG_HPP

#include "Level.hpp"
#include "Formatter.hpp"
#include "outputs/Output.hpp"
#include "iklog/outputs/OstreamWrapper.hpp"
#include <map>
#include <chrono>
#include <iostream>
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
         * \param output The output that will be used for all levels
         * \param formatter The formatter to use for the logging messages
         */
        IKLOG_EXPORT Log(const std::string& name, int levels, Output& output = DEFAULT_OUTPUT,
                         const Formatter& formatter = Formatter());


        IKLOG_EXPORT virtual ~Log();


        /*!
         * \brief Logs a message in the given level
         * \param level The level of the logging message
         * \param message The message to log
         */
        IKLOG_EXPORT virtual void log(Level level, const std::string& message) const;


        /*!
         * \brief Logs a message in INFO level
         * \param message The message to log
         */
        inline void info(const std::string& message) const { log(Level::INFO, message); }

        /*!
         * \brief Logs a message in DEBUG level
         * \param message The message to log
         */
        inline void debug(const std::string& message) const { log(Level::DEBUG, message); }

        /*!
         * \brief Logs a message in WARNING level
         * \param message The message to log
         */
        inline void warn(const std::string& message) const { log(Level::WARNING, message); }

        /*!
         * \brief Logs a message in ERROR level
         * \param message The message to log
         */
        inline void error(const std::string& message) const { log(Level::ERROR, message); }


        /*!
         * \brief Checks whether a level is enabled for this Log
         * \param level The level to check
         * \return True if the level is enabled
         */
        inline bool isLevelEnabled(Level level) const { return (m_levels & level) != 0; }


        /*!
         * \brief Enables the given levels
         * \param levels List of flags describing the levels to enable
         */
        inline void enableLevels(int levels) { m_levels |= levels; }

        /*!
         * \brief Disables the given levels
         * \param levels List of flags describing the levels to disable
         */
        inline void disableLevels(int levels) { m_levels &= ~levels; }


        /*!
         * \brief Gives the Log object having a specific name, or the iklog::NullLog if no Log has the given name
         * \param name The name of the Log to fetch
         * \return The Log with the given name if it has been found
         */
        IKLOG_EXPORT static Log* getLog(const std::string& name);


        /*!
         * \brief Changes the output for a specific level
         * \param level The level that will have a new output
         * \param output The output to use
         */
        inline void setOutput(Level level, Output& output) { m_outputs[level] = &output; }

        /*!
         * \brief Changes the output for all levels
         * \param output The output to use
         */
        void setOutput(Output& output)
        {
            for(const auto& outputEntry: m_outputs)
                m_outputs[outputEntry.first] = &output;
        }


        inline void setFormatter(const Formatter& formatter) { m_formatter = formatter; }

    protected:

        IKLOG_EXPORT static OstreamWrapper DEFAULT_OUTPUT; // The default output for logs when no output is provided

    private:

        inline static std::map<std::string, Log*>& getLogsList()
        {
            static std::map<std::string, Log*> logsList;
            return logsList;
        }


        const std::string m_name;
        std::map<Level, Output*> m_outputs;
        int m_levels;
        std::chrono::system_clock::time_point m_startTime;
        Formatter m_formatter;
};

}

#endif // IKLOG_LOG_HPP
