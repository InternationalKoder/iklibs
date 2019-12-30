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

#ifndef IKLOGCONF_LOG_CONFIGURATION_HPP
#define IKLOGCONF_LOG_CONFIGURATION_HPP

#include <ikconf/Configuration.hpp>
#include <ikconf/ConfigurationList.hpp>
#include <vector>
#include "LogConfigurationItem.hpp"
#include "../iklogconf_export.hpp"

namespace iklogconf
{
    /*!
     * \brief Represents the structure of a log configuration file
     */
    class LogConfiguration : public ikconf::Configuration
    {
        public:

            IKLOGCONF_EXPORT LogConfiguration();

            inline std::vector<LogConfigurationItem> getLogConfigurationItems() const { return m_logs.getProperties(); }

        private:

            ikconf::ConfigurationList<LogConfigurationItem> m_logs; // list of the log items
    };
}

#endif // IKLOGCONF_LOG_CONFIGURATION_HPP