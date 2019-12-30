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

#ifndef IKLOGCONF_LOG_CONFIGURATION_ITEM_HPP
#define IKLOGCONF_LOG_CONFIGURATION_ITEM_HPP

#include <ikconf/Configuration.hpp>
#include <string>
#include <vector>
#include "LogConfigurationOutput.hpp"

namespace iklogconf
{
    /*!
     * \brief Represents the structure of a log item from a log configuration file
     */
    class LogConfigurationItem : public ikconf::Configuration
    {
        public:

            LogConfigurationItem();

            inline std::string getName() const { return m_name; }
            inline LogConfigurationOutput getOutput() const { return m_output; }
            inline std::vector<std::string> getLevels() const { return m_levels; }

        private:

            std::string m_name; // log's name
            LogConfigurationOutput m_output; // the outputs where the messages will be written
            std::vector<std::string> m_levels; // logging levels
    };
}

#endif // IKLOGCONF_LOG_CONFIGURATION_ITEM_HPP
