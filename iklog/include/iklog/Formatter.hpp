/*
    Copyright (C) 2019, 2023, InternationalKoder

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

#ifndef IKLOG_FORMATTER_HPP
#define IKLOG_FORMATTER_HPP

#include <string>
#include <map>
#include "Message.hpp"
#include "iklog_export.hpp"

namespace iklog
{

/*!
 * \brief Makes the formatting of a log message
 *
 * Available fields in the format are:
 * - %L name of the iklog::Log object
 * - %l full name of the logging level
 * - %p logging level with a fixed number of characters
 * - %m log message
 * - %d duration from the creation of the iklog::Log object
 * - %t current clock time
 */
class Formatter
{
    public:

        /*!
         * \brief Constructor taking a format as a std::string
         * \param format The format to use for all the messages
         */
        IKLOG_EXPORT Formatter(const std::string& format = "%t %L [%p] %m");


        /*!
         * \brief Applies the format to the given message
         * \param message The log message to format
         * \return The formatted message
         */
        IKLOG_EXPORT std::string format(const Message& message) const;


        static inline std::string getLogName(const Message& message) { return message.getLogName(); }
        IKLOG_EXPORT static std::string getLevel(const Message& message);
        IKLOG_EXPORT static std::string getLevelPretty(const Message& message);
        static inline std::string getMessage(const Message& message) { return message.getMessage(); }
        IKLOG_EXPORT static std::string getProgramDuration(const Message& message);
        IKLOG_EXPORT static std::string getClockTime(const Message& message);

        inline void setFormat(const std::string& format) { m_format = format; }

    private:

        typedef std::string(*getFunc)(const Message&); // pointer to the getter methods

        static const std::map<char, getFunc> FORMAT_MAPPING; // mapping from the fields of the format to the getters

        std::string m_format; // the format to apply to all the messages
};

}

#endif // IKLOG_FORMATTER_HPP
