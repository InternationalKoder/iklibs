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

#include "ikconf/readers/JsonReader.hpp"

#include <fstream>
#include "ikconf/exceptions/ConfigurationException.hpp"

namespace ikconf
{
    iklog::Log JsonReader::LOG("JsonReader", iklog::Level::WARNING);


    JsonReader::JsonReader(const Configuration& configuration) :
        BaseReader(configuration),
        m_lineNumber(1)
    {}


    void JsonReader::read(const std::string& filePath)
    {
        std::ifstream file(filePath.c_str());
        char character;
        m_lineNumber = 1;

        if(!file.is_open())
            throw ConfigurationException("Cannot open file '" + filePath + "'");

        std::string name, value;

        character = readCharacter(file);

        if(character == '{')
        {
            readObject(file, m_configuration);
        }
        else if(character == '[')
        {
            throw ConfigurationException("Array on base level is not supported, please insert the array in a JSON object");
        }
        else
            handleUnexpectedCharacter(character);

        file.close();
    }


    void JsonReader::readObject(std::ifstream& file, Configuration& configuration)
    {
        char character = ',';
        std::string propertyName, propertyValue;
        bool setPropertySuccess = false;

        while(character == ',')
        {
            propertyName = "";
            propertyValue = "";

            character = readCharacter(file);
            if(character != '"')
                handleUnexpectedCharacter(character);

            do
            {
                character = readCharacter(file);
                propertyName += character;
            } while(character != '"');
            propertyName.pop_back();

            character = readCharacter(file);
            if(character != ':')
                handleUnexpectedCharacter(character);

            // check if the property is known
            if(!configuration.checkPropertyExists(propertyName))
            {
                LOG.warn("Unknown property '" + propertyName + "' was skipped");
                unsigned int level = 1;
                bool insideString = false;
                while((character != ',' && level > 0) || insideString)
                {
                    character = readCharacter(file, true);
                    if(character == '[' || character == '{')
                        level++;
                    else if(character == ']' || character == '}')
                        level--;
                    else if(character == '"')
                        insideString = !insideString;
                }
                continue;
            }

            // assign value to property
            character = readCharacter(file);
            if(character == '{')
            {
                // value is an object
                Configuration* subConfig = std::any_cast<Configuration*>(configuration.getPropertyValue(propertyName));
                readObject(file, *subConfig);
                character = readCharacter(file);
            }
            else if(character == '[')
            {
                // value is an array
                readArray(file, configuration, propertyName);
                character = readCharacter(file);
            }
            else
            {
                // value is a basic type
                propertyValue += character;

                file.unsetf(std::ios_base::skipws);
                while(file >> character && character != ',' && character != '}')
                    propertyValue += character;
                file.setf(std::ios_base::skipws);

                propertyValue = trim(propertyValue);

                if(propertyValue.front() == '"' && propertyValue.back() == '"')
                {
                    propertyValue.pop_back();
                    propertyValue = propertyValue.substr(1);
                }

                setPropertySuccess = tryConvertAndSetProperty(propertyName, propertyValue, configuration);
            }

            if(!setPropertySuccess)
                throw ConfigurationException("Failed to set property '" + propertyName + "'");
        }

        if(character != '}')
            handleUnexpectedCharacter(character);
    }


    void JsonReader::readArray(std::ifstream& file, Configuration& configuration, const std::string& propertyName)
    {
        std::string propertyValue;
        char character = '[';
        bool addPropertySuccess = true;

        while(character != ']' && addPropertySuccess)
        {
            // read the value
            propertyValue = "";
            file.unsetf(std::ios_base::skipws);
            while(file >> character && character != ',' && character != ']')
                propertyValue += character;
            file.setf(std::ios_base::skipws);

            // arrange the value before inserting it
            propertyValue = trim(propertyValue);

            if(propertyValue.front() == '"' && propertyValue.back() == '"')
            {
                propertyValue.pop_back();
                propertyValue = propertyValue.substr(1);
            }

            // insert the value
            addPropertySuccess = tryConvertAndSetProperty(propertyName, propertyValue, configuration);
        }
    }


    void JsonReader::handleUnexpectedCharacter(char character)
    {
        std::string errorMsg = "Unexpected character '";
        errorMsg.push_back(character);
        errorMsg += "' at line " + std::to_string(m_lineNumber);

        throw ConfigurationException(errorMsg);
    }


    char JsonReader::readCharacter(std::ifstream& file, bool acceptEof)
    {
        char character = '\0';

        file.unsetf(std::ios_base::skipws);
        while(file >> character && (character == ' ' || character == '\n'
                                    || character == '\r' || character == '\t' || character == '\0'))
        {
            if(character == '\n')
                m_lineNumber++;
        }
        file.setf(std::ios_base::skipws);

        if(character == '\0' && !acceptEof)
            throw ConfigurationException("Unexpected end of file");

        return character;
    }
}
