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

#include "ikconf/readers/JsonReader.hpp"

#include "ikconf/readers/BufferedFile.hpp"

namespace ikconf
{

JsonReader::JsonReader(const Configuration& configuration) :
    BaseReader(configuration),
    m_lineNumber(1)
{}


ikgen::Result<std::vector<Warning>, std::string> JsonReader::read(const std::string& filePath)
{
    BufferedFile file(filePath.c_str());
    char character;
    m_lineNumber = 1;

    if(!file.isOpen())
        return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Cannot open file '" + filePath + "'");

    std::string name, value;

    ikgen::Result<char, std::string> readCharacterResult = readCharacter(file);
    if(readCharacterResult.isFailure())
        return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
    character = readCharacterResult.getSuccess();

    if(character == '{')
    {
        return readObject(file, m_configuration);
    }
    else if(character == '[')
    {
        return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Array on base level is not supported, please insert the array in a JSON object");
    }
    else
        return ikgen::Result<std::vector<Warning>, std::string>::failure(buildUnexpectedCharacterMessage(character));
}


ikgen::Result<std::vector<Warning>, std::string> JsonReader::readObject(BufferedFile& file, Configuration& configuration)
{
    std::vector<Warning> warnings;
    char character = ',';
    std::string propertyName, propertyValue;
    bool setPropertySuccess = false;
    ikgen::Result<char, std::string> readCharacterResult = ikgen::Result<char, std::string>::makeSuccess(',');

    while(character == ',')
    {
        propertyName = "";
        propertyValue = "";

        readCharacterResult = readCharacter(file);
        if(readCharacterResult.isFailure())
            return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
        character = readCharacterResult.getSuccess();

        if(character != '"')
            return ikgen::Result<std::vector<Warning>, std::string>::failure(buildUnexpectedCharacterMessage(character));

        do
        {
            readCharacterResult = readCharacter(file);
            if(readCharacterResult.isFailure())
                return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
            character = readCharacterResult.getSuccess();

            propertyName += character;
        } while(character != '"');
        propertyName.pop_back();

        readCharacterResult = readCharacter(file);
        if(readCharacterResult.isFailure())
            return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
        character = readCharacterResult.getSuccess();

        if(character != ':')
            return ikgen::Result<std::vector<Warning>, std::string>::failure(buildUnexpectedCharacterMessage(character));

        // check if the property is known
        if(!configuration.checkPropertyExists(propertyName))
        {
            warnings.emplace_back(Warning::Type::SKIPPED_UNKNOWN_PROPERTY, "Unknown property '" + propertyName + "' was skipped");
            unsigned int level = 1;
            bool insideString = false;
            while((character != ',' && level > 0) || insideString)
            {
                readCharacterResult = readCharacter(file, true);
                if(readCharacterResult.isFailure())
                    return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
                character = readCharacterResult.getSuccess();

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
        readCharacterResult = readCharacter(file);
        if(readCharacterResult.isFailure())
            return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
        character = readCharacterResult.getSuccess();

        if(character == '{')
        {
            // value is an object
            Configuration* subConfig = std::any_cast<Configuration*>(configuration.getPropertyValue(propertyName));
            readObject(file, *subConfig);

            readCharacterResult = readCharacter(file);
            if(readCharacterResult.isFailure())
                return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
            character = readCharacterResult.getSuccess();

            setPropertySuccess = true;
        }
        else if(character == '[')
        {
            // value is an array
            readArray(file, configuration, propertyName);

            readCharacterResult = readCharacter(file);
            if(readCharacterResult.isFailure())
                return ikgen::Result<std::vector<Warning>, std::string>::failure(std::move(readCharacterResult.getFailure()));
            character = readCharacterResult.getSuccess();

            setPropertySuccess = true;
        }
        else
        {
            // value is a basic type
            propertyValue += character;

            character = file.nextChar();
            while(character != ',' && character != '}')
            {
                propertyValue += character;
                character = file.nextChar();
            }

            propertyValue = trim(propertyValue);

            if(propertyValue.front() == '"' && propertyValue.back() == '"')
            {
                propertyValue.pop_back();
                propertyValue = propertyValue.substr(1);
            }

            setPropertySuccess = tryConvertAndSetProperty(propertyName, propertyValue, configuration);
        }

        if(!setPropertySuccess)
            return ikgen::Result<std::vector<Warning>, std::string>::makeFailure("Failed to set property '" + propertyName + "'");
    }

    if(character != '}')
        return ikgen::Result<std::vector<Warning>, std::string>::failure(buildUnexpectedCharacterMessage(character));

    return ikgen::Result<std::vector<Warning>, std::string>::success(std::move(warnings));
}


ikgen::Result<ikgen::EmptyResult, std::string> JsonReader::readArray(BufferedFile& file, Configuration& configuration, const std::string& propertyName)
{
    std::string propertyValue;
    char character = '[';
    bool addPropertySuccess = true;

    while(character != ']' && addPropertySuccess)
    {
        // read the value
        propertyValue = "";
        character = file.nextChar();
        while(character != ',' && character != ']' && character != '{')
        {
            propertyValue += character;
            character = file.nextChar();
        }

        if(character == '{')
        {
            // value is an object
            Configuration* const listConfig = std::any_cast<Configuration*>(configuration.getPropertyValue(propertyName));
            Configuration* const subConfig = listConfig->newListItem();
            readObject(file, *subConfig);

            auto readCharacterResult = readCharacter(file);
            if(readCharacterResult.isFailure())
                return ikgen::Result<ikgen::EmptyResult, std::string>::failure(std::move(readCharacterResult.getFailure()));
            character = readCharacterResult.getSuccess();
        }
        else
        {
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

    return ikgen::Result<ikgen::EmptyResult, std::string>::makeSuccess();
}


std::string JsonReader::buildUnexpectedCharacterMessage(char character)
{
    std::string errorMsg = "Unexpected character '";
    errorMsg.push_back(character);
    errorMsg += "' at line " + std::to_string(m_lineNumber);

    return errorMsg;
}


ikgen::Result<char, std::string> JsonReader::readCharacter(BufferedFile& file, bool acceptEof)
{
    char character = '\0';

    while(character == ' ' || character == '\n'
          || character == '\r' || character == '\t' || character == '\0')
    {
        character = file.nextChar();
        if(character == '\n')
            ++m_lineNumber;
    }

    if(character == '\0' && !acceptEof)
        return ikgen::Result<char, std::string>::makeFailure("Unexpected end of file");

    return ikgen::Result<char, std::string>::success(std::move(character));
}

}
