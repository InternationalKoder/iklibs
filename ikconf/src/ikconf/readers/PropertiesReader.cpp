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

#include "ikconf/readers/PropertiesReader.hpp"

#include <fstream>
#include "ikconf/exceptions/ConfigurationException.hpp"

namespace ikconf
{
#ifdef IKCONF_USE_IKLOG
    iklog::Log PropertiesReader::LOG("PropertiesReader", iklog::Level::WARNING);
#endif

    PropertiesReader::PropertiesReader(const Configuration& configuration) :
        BaseReader(configuration)
    {}

    void PropertiesReader::read(const std::string& filePath)
    {
        std::ifstream file(filePath.c_str());
        std::string line;

        if(!file.is_open())
            throw ConfigurationException("Cannot open file '" + filePath + "'");

        while(getline(file, line))
        {
            // don't go further if the line is empty
            if(line.length() > 0)
            {
                // don't interpret comments
                if(line.front() != '#' && line.front() != '!')
                {
                    size_t separatorPos = line.find_first_of('=');

                    // '=' character not found means the line is malformed
                    if(separatorPos == std::string::npos)
                        throw ConfigurationException("Malformed line '" + line + "', missing '=' character");

                    const std::string propertyName = line.substr(0, separatorPos);

                    // check if the property is known
                    if(!m_configuration.checkPropertyExists(propertyName))
                    {
                        logWarningMessage("Unknown property '" + propertyName + "' was skipped");
                        continue;
                    }

                    const std::any propertyValue = line.substr(separatorPos + 1);

                    const bool setPropertySuccess = tryConvertAndSetProperty(propertyName, propertyValue);

                    if(!setPropertySuccess)
                        throw ConfigurationException("Failed to set property '" + propertyName + "'");
                }
            }
        }

        file.close();
    }
}
