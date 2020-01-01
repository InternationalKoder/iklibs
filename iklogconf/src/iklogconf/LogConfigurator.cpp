/*
    Copyright (C) 2019, 2020, InternationalKoder

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

#include "iklogconf/LogConfigurator.hpp"
#include <ikconf/readers/JsonReader.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>
#include "iklogconf/config-model/LogConfiguration.hpp"
#include <optional>

namespace iklogconf
{
    void LogConfigurator::readJsonFile(const std::string& filepath)
    {
        // Read configuration file
        LogConfiguration configuration;
        ikconf::JsonReader jsonReader(configuration);
        jsonReader.read(filepath);

        // Create loggers from configuration
        const std::vector<LogConfigurationItem>& configItems = configuration.getLogConfigurationItems();
        std::for_each(configItems.begin(), configItems.end(),
                      [this](const LogConfigurationItem& item) { createLogger(item); });
    }

    void LogConfigurator::createLogger(const LogConfigurationItem& configItem)
    {
        if(configItem.getName().size() == 0)
            return;

        // Interpret the levels
        int levels = 0;
        const std::vector<std::string>& configLevels = configItem.getLevels();
        for(const std::string& level : configLevels)
        {
            if(level == "ERROR")
                levels |= iklog::Level::ERROR;
            else if(level == "WARNING")
                levels |= iklog::Level::WARNING;
            else if(level == "INFO")
                levels |= iklog::Level::INFO;
            else if(level == "DEBUG")
                levels |= iklog::Level::DEBUG;
        }

        // Interpret the output type
        std::optional<iklog::Output*> output;
        const LogConfigurationOutput& configOutput = configItem.getOutput();
        const std::string& configOutputType = configOutput.getType();
        if(configOutputType == "stdout")
            output = &iklog::OstreamWrapper::COUT;
        else if(configOutputType == "stderr")
            output = &iklog::OstreamWrapper::CERR;
        else if(configOutputType == "stdlog")
            output = &iklog::OstreamWrapper::CLOG;
        else if(configOutputType == "rolling-file")
        {
            // Rolling file output
            const std::string& configMaxFileSize = configOutput.getMaxFileSize();
            iklog::FileSize maxFileSize = iklog::Bytes(0);

            if(configMaxFileSize.size() > 1)
            {
                if(configMaxFileSize.size() > 2)
                {
                    const std::string unit = configMaxFileSize.substr(configMaxFileSize.size() - 2);
                    const std::string value = configMaxFileSize.substr(0, configMaxFileSize.size() - 2);
                    if(unit == "kb")
                        maxFileSize = iklog::KiloBytes(static_cast<uintmax_t>(std::stoi(value))).toBytes();
                    else if(unit == "mb")
                        maxFileSize = iklog::MegaBytes(static_cast<uintmax_t>(std::stoi(value))).toBytes();
                    else if(unit == "gb")
                        maxFileSize = iklog::GigaBytes(static_cast<uintmax_t>(std::stoi(value))).toBytes();
                    else if(unit[1] == 'b')
                        maxFileSize = iklog::Bytes(static_cast<uintmax_t>(std::stoi(value)));
                }
                else if(configMaxFileSize.at(configMaxFileSize.size() - 1) == 'b')
                {
                    maxFileSize = iklog::Bytes(static_cast<uintmax_t>(
                                            std::stoi(configMaxFileSize.substr(0, configMaxFileSize.size() - 1))));
                }
            }

            if(maxFileSize.getValueInBytes() > 0 && configOutput.getBaseFileName().size() > 0 &&
                    configOutput.getMaxRollingFiles() > 0)
            {
                m_outputs.push_back(std::make_unique<iklog::RollingFileOutput>(
                            configOutput.getBaseFileName(), maxFileSize, configOutput.getMaxRollingFiles()));
                output = m_outputs.back().get();
            }
        }

        // Interpret the format
        std::optional<iklog::Formatter> formatter;
        if(configItem.getFormat() != "")
            formatter = iklog::Formatter(configItem.getFormat());

        // Create log
        if(output.has_value())
        {
            if(formatter.has_value())
                m_logs.emplace_back(std::make_unique<iklog::Log>(configItem.getName(), levels, *(output.value()), formatter.value()));
            else
                m_logs.emplace_back(std::make_unique<iklog::Log>(configItem.getName(), levels, *(output.value())));
        }
        else
            m_logs.emplace_back(std::make_unique<iklog::Log>(configItem.getName(), levels));
    }
}
