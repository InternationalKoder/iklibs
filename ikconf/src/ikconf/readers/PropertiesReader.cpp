/*
    Copyright (C) 2019, 2021, 2023, InternationalKoder

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

#include "ikconf/readers/BufferedFile.hpp"

namespace ikconf
{

PropertiesReader::PropertiesReader(const Configuration& configuration) :
    BaseReader(configuration)
{}

ikgen::Result<std::vector<Warning>, std::string> PropertiesReader::read(const std::string& filePath)
{
    BufferedFile file(filePath.c_str());
    std::string line;

    if(!file.isOpen())
        return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Cannot open file '" + filePath + "'");

    std::vector<Warning> warnings;

    while(!file.isEof())
    {
        line = file.nextLine();

        // don't go further if the line is empty
        if(line.length() > 0)
        {
            // don't interpret comments
            if(line.front() != '#' && line.front() != '!')
            {
                size_t separatorPos = line.find_first_of('=');

                // '=' character not found means the line is malformed
                if(separatorPos == std::string::npos)
                    return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Malformed line '" + line + "', missing '=' character");

                const std::string propertyName = line.substr(0, separatorPos);

                // check if the property is known
                if(!m_configuration.checkPropertyExists(propertyName))
                {
                    warnings.emplace_back(Warning::Type::SKIPPED_UNKNOWN_PROPERTY, "Unknown property '" + propertyName + "' was skipped");
                    continue;
                }

                const std::any propertyValue = line.substr(separatorPos + 1);

                const bool setPropertySuccess = tryConvertAndSetProperty(propertyName, propertyValue);

                if(!setPropertySuccess)
                    return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Failed to set property '" + propertyName + "'");
            }
        }
    }

    return ikgen::Result<std::vector<Warning>, std::string>::success(std::move(warnings));
}

}
