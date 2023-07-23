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
#include "iklogconf/config-model/LogConfiguration.hpp"
#include <ikconf/readers/JsonReader.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>
#include <algorithm>

namespace iklogconf
{

ikgen::Result<std::vector<Warning>, std::string> LogConfigurator::readJsonFile(const std::string& filepath)
{
    // Read configuration file
    LogConfiguration configuration;
    ikconf::JsonReader jsonReader;
    auto readResult = jsonReader.read(filepath, configuration);

    if(readResult.isFailure())
        return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readResult.getFailure()));

    std::vector<Warning> warnings;

    // Add warnings from previous read
    warnings.reserve(readResult.getSuccess().size());
    std::ranges::transform(readResult.getSuccess(), std::back_inserter(warnings),
                           [](const ikconf::Warning& readWarning)
                           {
                               return Warning(Warning::Type::SKIPPED_CONFIGURATION_PROPERTY, std::string(readWarning.getMessage()));
                           });

    // Create outputs from configuration
    const std::vector<OutputConfigurationItem>& outputItems = configuration.getOutputConfigurationItems();
    for(const OutputConfigurationItem& item : outputItems)
    {
        std::optional<Warning> createOutputWarning = createOutput(item);
        if(createOutputWarning.has_value())
            warnings.push_back(std::move(createOutputWarning.value()));
    }

    // Create loggers from configuration
    const std::vector<LogConfigurationItem>& logItems = configuration.getLogConfigurationItems();
    for(const LogConfigurationItem& item : logItems)
    {
        std::vector<Warning> createLoggerWarnings = createLogger(item);
        warnings.insert(warnings.end(), createLoggerWarnings.begin(), createLoggerWarnings.end());
    }

    return ikgen::Result<std::vector<Warning>, std::string>::success(std::move(warnings));
}

std::optional<Warning> LogConfigurator::createOutput(const OutputConfigurationItem& configItem)
{
    ikgen::Result<iklog::Output*, Warning> output = createOutputBase(configItem);

    if(output.isSuccess() && configItem.getName() != "")
    {
        m_namedOutputs[configItem.getName()] = output.getSuccess();
        return std::nullopt;
    }
    else if(output.isSuccess())
        return std::make_optional<Warning>(Warning::Type::OUTPUT_CREATION_FAILURE, "Skipped creation of unnamed output");
    else
        return std::optional<Warning>(std::move(output.getFailure()));
}

std::vector<Warning> LogConfigurator::createLogger(const LogConfigurationItem& configItem)
{
    std::vector<Warning> warnings;

    if(configItem.getName().size() == 0)
        return warnings;

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
        else
            warnings.emplace_back(Warning::Type::UNKNOWN_LEVEL, "Level '" + level + "' does not exist");
    }

    // Interpret the output or output-ref
    std::optional<iklog::Output*> output;
    if(configItem.getOutputRef() != "")
    {
        // Try to find the named output, add warning if it is not found
        auto it = m_namedOutputs.find(configItem.getOutputRef());
        if(it != m_namedOutputs.end())
            output = it->second;
        else
            warnings.emplace_back(Warning::Type::OUTPUT_NOT_FOUND, "Could not find an output with name '" + configItem.getOutputRef() + "'");
    }
    else
    {
        auto createResult = createOutputBase(configItem.getOutput());
        if(createResult.isSuccess())
            output = std::move(createResult.getSuccess());
        else
            warnings.push_back(createResult.getFailure());
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

    return warnings;
}

ikgen::Result<iklog::Output*, Warning> LogConfigurator::createOutputBase(const LogConfigurationOutput& configItem)
{
    iklog::Output* output = nullptr;
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
                else
                    return ikgen::Result<iklog::Output*, Warning>::makeFailure(
                        Warning::Type::INCORRECT_FILE_SIZE, "Cound not create rolling file output with incorrect max file size '" + configMaxFileSize + "'");
            }
            else if(configMaxFileSize[configMaxFileSize.size() - 1] == 'b')
            {
                maxFileSize = iklog::Bytes(static_cast<uintmax_t>(
                                        std::stoi(configMaxFileSize.substr(0, configMaxFileSize.size() - 1))));
            }
            else
                return ikgen::Result<iklog::Output*, Warning>::makeFailure(
                    Warning::Type::INCORRECT_FILE_SIZE, "Cound not create rolling file output with incorrect max file size '" + configMaxFileSize + "'");
        }

        // Actual creation of the iklog::RollingFileOutput
        if(maxFileSize.getValueInBytes() > 0 && configItem.getBaseFileName().size() > 0 &&
                configItem.getMaxRollingFiles() > 0)
        {
            try
            {
                m_outputs.push_back(std::make_unique<iklog::RollingFileOutput>(
                            configItem.getBaseFileName(), maxFileSize, configItem.getMaxRollingFiles()));
            }
            catch(const std::runtime_error& e)
            {
                return ikgen::Result<iklog::Output*, Warning>::makeFailure(Warning::Type::OUTPUT_CREATION_FAILURE, e.what());
            }

            output = m_outputs.back().get();
        }
    }
    else
        return ikgen::Result<iklog::Output*, Warning>::makeFailure(
            Warning::Type::OUTPUT_CREATION_FAILURE, "Cound not create output of unknown type '" + configOutputType + "'");

    if(output == nullptr)
        return ikgen::Result<iklog::Output*, Warning>::makeFailure(Warning::Type::OUTPUT_CREATION_FAILURE, "Failed to create output");

    return ikgen::Result<iklog::Output*, Warning>::success(std::move(output));
}

}
