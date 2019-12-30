/*
    Copyright (C) 2019, InternationalKoder

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

#ifndef IKLOGCONF_LOG_CONFIGURATION_OUTPUT_HPP
#define IKLOGCONF_LOG_CONFIGURATION_OUTPUT_HPP

#include <ikconf/Configuration.hpp>
#include <string>

namespace iklogconf
{
    /*!
     * \brief Represents the structure of an output item from a log configuration file
     */
    class LogConfigurationOutput : public ikconf::Configuration
    {
        public:

            LogConfigurationOutput();

            inline std::string getType() const { return m_type; }
            inline std::string getBaseFileName() const { return m_baseFileName; }
            inline std::string getMaxFileSize() const { return m_maxFileSize; }
            inline unsigned int getMaxRollingFiles() const { return m_maxRollingFiles; }

        private:

            std::string m_type; // the type of the output
            std::string m_baseFileName; // base name for the log files (used when relevant)
            std::string m_maxFileSize; // maximum size for the log files (used when relevant)
            unsigned int m_maxRollingFiles; // maximum number of rolling log files (used when relevant)
    };
}

#endif // IKLOGCONF_LOG_CONFIGURATION_OUTPUT_HPP
