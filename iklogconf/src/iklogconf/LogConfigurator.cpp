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

#include "iklogconf/LogConfigurator.hpp"
#include <ikconf/readers/JsonReader.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>
#include "iklogconf/config-model/LogConfiguration.hpp"

namespace iklogconf
{

ikgen::Result<std::vector<ikconf::Warning>, std::string> LogConfigurator::readJsonFile(const std::string& filepath)
{
    // Read configuration file
    LogConfiguration configuration;
    ikconf::JsonReader jsonReader(configuration);
    auto readResult = jsonReader.read(filepath);

    if(readResult.isFailure())
        return ikgen::Result<std::vector<ikconf::Warning>, std::string>::failure(std::move(readResult.getFailure()));

    // Create outputs from configuration
    const std::vector<OutputConfigurationItem>& outputItems = configuration.getOutputConfigurationItems();
    std::for_each(outputItems.begin(), outputItems.end(),
                  [this](const OutputConfigurationItem& item) { createOutput(item); });

    // Create loggers from configuration
    const std::vector<LogConfigurationItem>& logItems = configuration.getLogConfigurationItems();
    std::for_each(logItems.begin(), logItems.end(),
                  [this](const LogConfigurationItem& item) { createLogger(item); });

    return ikgen::Result<std::vector<ikconf::Warning>, std::string>::success(std::move(readResult.getSuccess()));
}

void LogConfigurator::createOutput(const OutputConfigurationItem& configItem)
{
    std::optional<iklog::Output*> output = createOutputBase(configItem);

    if(output.has_value() && configItem.getName() != "")
        m_namedOutputs[configItem.getName()] = output.value();
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

    // Interpret the output or output-ref
    std::optional<iklog::Output*> output;
    if(configItem.getOutputRef() != "")
    {
        // Try to find the named output, ignore if it is not found
        auto it = m_namedOutputs.find(configItem.getOutputRef());
        if(it != m_namedOutputs.end())
            output = it->second;
    }
    else
        output = createOutputBase(configItem.getOutput());

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

std::optional<iklog::Output*> LogConfigurator::createOutputBase(const LogConfigurationOutput& configItem)
{
    std::optional<iklog::Output*> output;
    const std::string& configOutputType = configItem.getType();

    if(configOutputType == "stdout")
        output = &iklog::OstreamWrapper::COUT;
    else if(configOutputType == "stderr")
        output = &iklog::OstreamWrapper::CERR;
    else if(configOutputType == "stdlog")
        output = &iklog::OstreamWrapper::CLOG;
    else if(configOutputType == "rolling-file")
    {
        // Rolling file output
        const std::string& configMaxFileSize = configItem.getMaxFileSize();
        iklog::FileSize maxFileSize = iklog::Bytes(0);

        // Read the maximum file size
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
            else if(configMaxFileSize[configMaxFileSize.size() - 1] == 'b')
            {
                maxFileSize = iklog::Bytes(static_cast<uintmax_t>(
                                        std::stoi(configMaxFileSize.substr(0, configMaxFileSize.size() - 1))));
            }
        }

        // Actual creation of the iklog::RollingFileOutput
        if(maxFileSize.getValueInBytes() > 0 && configItem.getBaseFileName().size() > 0 &&
                configItem.getMaxRollingFiles() > 0)
        {
            m_outputs.push_back(std::make_unique<iklog::RollingFileOutput>(
                        configItem.getBaseFileName(), maxFileSize, configItem.getMaxRollingFiles()));
            output = m_outputs.back().get();
        }
    }

    return output;
}

}
