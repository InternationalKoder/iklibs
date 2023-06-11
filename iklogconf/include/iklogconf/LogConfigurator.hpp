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

#ifndef IKLOGCONF_LOG_CONFIGURATOR_HPP
#define IKLOGCONF_LOG_CONFIGURATOR_HPP

#include "iklogconf_export.hpp"
#include "Warning.hpp"
#include <string>
#include <vector>
#include <memory>
#include <iklog/Log.hpp>
#include <ikgen/Result.hpp>

namespace iklogconf
{

class LogConfigurationItem;
class LogConfigurationOutput;
class OutputConfigurationItem;

/*!
 * \brief Configures the logging system from a JSON file, by reading the file and creating the
 * described iklog::Log instances
 */
class LogConfigurator
{
    public:

        /*!
         * \brief Creates the logging system described in the given JSON file
         * \param filePath Path to the JSON file
         * \return The warnings that may have been raised while reading the JSON file, or an error message
         */
        IKLOGCONF_EXPORT ikgen::Result<std::vector<Warning>, std::string> readJsonFile(const std::string& filePath);

    private:

        /*!
         * \brief Create a named iklog::Output instance from an item described in the configuration file
         * \param configItem The item read from the file
         * \return A warning, if one is raised while creating the output
         */
        std::optional<Warning> createOutput(const OutputConfigurationItem& configItem);

        /*!
         * \brief Create a iklog::Log instance from an item described in the configuration file
         * \param configItem The item read from the file
         * \return A list of warnings
         */
        std::vector<Warning> createLogger(const LogConfigurationItem& configItem);

        /*!
         * \brief Create a iklog::Output instance from an item described in the configuration file
         * \param configItem The item read from the file
         * \return The created iklog::Output if the description in the configuration file is valid, or a warning
         */
        ikgen::Result<iklog::Output*, Warning> createOutputBase(const LogConfigurationOutput& configItem);


        std::vector<std::unique_ptr<iklog::Log>> m_logs; // maintain a list of the allocated iklog::Log instances
        std::vector<std::unique_ptr<iklog::Output>> m_outputs; // maintain a list of the allocated iklog::Output instances
        std::map<std::string, iklog::Output*> m_namedOutputs; // outputs created with an explicit name
};

}

#endif // IKLOGCONF_LOG_CONFIGURATOR_HPP
