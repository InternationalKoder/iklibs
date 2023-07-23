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

using ReadResult = ikgen::Result<std::vector<Warning>, std::string>;
using ReadValueResult = ikgen::Result<std::string, std::string>;

enum class ReadStep { BetweenValues, BeforeName, BeforeValue };
enum class NodeType { Object, Array };

struct Node
{
    NodeType type;
    Configuration* value;
};


ReadResult JsonReader::read(const std::string& filePath, Configuration& configuration)
{
    // Open File
    BufferedFile file(filePath.c_str());
    m_lineNumber = 1;

    if(!file.isOpen())
        return ReadResult::makeFailure("Cannot open file '" + filePath + "'");

    // Read JSON
    std::vector<Node> nodeStack;
    std::optional<char> nextCharacter;

    // Initialize stack
    ikgen::Result<char, std::string> readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);
    if(readCharacterResult.isFailure())
        return ReadResult::failure(std::move(readCharacterResult.getFailure()));

    char character = readCharacterResult.getSuccess();
    if(character == '{')
        nodeStack.push_back(Node { .type = NodeType::Object, .value = &configuration });
    else if(character == '[')
        nodeStack.push_back(Node { .type = NodeType::Array, .value = &configuration });
    else
        return ReadResult::failure(buildUnexpectedCharacterMessage(character));

    ReadStep step = ReadStep::BetweenValues;
    std::optional<std::string> propertyName;
    std::vector<Warning> warnings;

    // Read values
    do
    {
        Configuration* const configuration = nodeStack.back().value;
        const NodeType nodeType = nodeStack.back().type;
        ReadValueResult readStringResult = ReadValueResult::failure("Unknown error");

        switch(step)
        {
            case ReadStep::BetweenValues:
                readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                if(readCharacterResult.isFailure())
                    return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                character = readCharacterResult.getSuccess();

                if((character == '}' && nodeType == NodeType::Object) ||
                    (character == ']' && nodeType == NodeType::Array))
                {
                    nodeStack.pop_back();
                }
                else
                {
                    if(character != ',')
                        nextCharacter = character;

                    step = nodeType == NodeType::Object ? ReadStep::BeforeName : ReadStep::BeforeValue;
                }
                break;

            case ReadStep::BeforeName:
                // Read until property name
                readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                if(readCharacterResult.isFailure())
                    return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                nextCharacter = readCharacterResult.getSuccess();

                // Read the property name
                readStringResult = readString(file, nextCharacter);

                if(readStringResult.isFailure())
                    return ReadResult::makeFailure(std::move(readStringResult.getFailure()));

                propertyName = readStringResult.getSuccess();

                // Read until after the ':' separator
                readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                if(readCharacterResult.isFailure())
                    return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                character = readCharacterResult.getSuccess();

                if(character != ':')
                    return ReadResult::failure(buildUnexpectedCharacterMessage(character));

                step = ReadStep::BeforeValue;
                break;

            case ReadStep::BeforeValue:
                // Make sure that we are in a correct state: the property name should have been read and
                // should exist in configuration for an object
                if(nodeType == NodeType::Object &&
                    (!propertyName.has_value() || (configuration != nullptr && !configuration->checkPropertyExists(*propertyName))))
                {
                    warnings.emplace_back(Warning::Type::SKIPPED_UNKNOWN_PROPERTY, "Unknown property '" + propertyName.value_or("") + "' was skipped");
                    propertyName.reset();
                }

                // Read value's first character to know what kind of value it is
                readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                if(readCharacterResult.isFailure())
                    return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                character = readCharacterResult.getSuccess();

                // Read and handle the full value
                if(character == '{')
                {
                    Configuration* subConfig = nullptr;

                    // Get the object's property in defined configuration
                    if(configuration != nullptr)
                    {
                        // Object inside an object
                        if(nodeType == NodeType::Object && propertyName.has_value() &&
                            configuration->checkPropertyExists(*propertyName))
                        {
                            subConfig = std::any_cast<Configuration*>(configuration->getPropertyValue(*propertyName));
                        }

                        // Array of objects == array of configurations
                        else if(nodeType == NodeType::Array)
                            subConfig = configuration->newListItem();
                    }

                    nodeStack.push_back(Node { .type = NodeType::Object, .value = subConfig });
                    step = ReadStep::BetweenValues;
                }
                else if(character == '[')
                {
                    // Check whether it is an array of configurations
                    readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                    if(readCharacterResult.isFailure())
                        return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                    character = readCharacterResult.getSuccess();
                    nextCharacter = character;

                    Configuration* subConfig = configuration;

                    // Get the array's property in defined configuration
                    if(character == '{' && configuration != nullptr && propertyName.has_value() &&
                        configuration->checkPropertyExists(*propertyName))
                    {
                        subConfig = std::any_cast<Configuration*>(configuration->getPropertyValue(*propertyName));
                    }

                    nodeStack.push_back(Node { .type = NodeType::Array, .value = subConfig });
                    step = ReadStep::BetweenValues;
                }
                else if(character == '"')
                {
                    nextCharacter = character;
                    auto readValueResult = readString(file, nextCharacter);
                    if(readValueResult.isFailure())
                        return ReadResult::makeFailure(readValueResult.getFailure());

                    if(propertyName.has_value() && configuration != nullptr &&
                        !tryConvertAndSetProperty(*propertyName, readValueResult.getSuccess(), *configuration))
                    {
                        return ReadResult::makeFailure("Failed to set property '" + *propertyName + "'");
                    }

                    // Read until we get to values separator
                    readCharacterResult = skipWhitespaceAndReadChar(file, nextCharacter);

                    if(readCharacterResult.isFailure())
                        return ReadResult::failure(std::move(readCharacterResult.getFailure()));
                    character = readCharacterResult.getSuccess();

                    if(character != ',' && (character != '}' || nodeType != NodeType::Object) && (character != ']' || nodeType != NodeType::Array))
                        return ReadResult::makeFailure(buildUnexpectedCharacterMessage(character));

                    if(character == ',')
                        step = nodeType == NodeType::Object ? ReadStep::BeforeName : ReadStep::BeforeValue;
                    else
                    {
                        nextCharacter = character;
                        step = ReadStep::BetweenValues;
                    }
                }
                else
                {
                    nextCharacter = character;
                    auto readValueResult = readBasicValue(file, nextCharacter, nodeType == NodeType::Array);
                    if(readValueResult.isFailure())
                        return ReadResult::makeFailure(readValueResult.getFailure());

                    if(!nextCharacter.has_value())
                        return ReadResult::makeFailure(buildUnexpectedCharacterMessage(character));

                    if(propertyName.has_value() && configuration != nullptr &&
                        !tryConvertAndSetProperty(*propertyName, trim(readValueResult.getSuccess()), *configuration))
                    {
                        return ReadResult::makeFailure("Failed to set property '" + *propertyName + "'");
                    }

                    // Handle values separator
                    character = readChar(file, nextCharacter);
                    if(character == ',')
                        step = nodeType == NodeType::Object ? ReadStep::BeforeName : ReadStep::BeforeValue;
                    else
                    {
                        nextCharacter = character;
                        step = ReadStep::BetweenValues;
                    }
                }

                break;
        }

    } while(!nodeStack.empty());

    return ReadResult::success(std::move(warnings));
}


std::string JsonReader::buildUnexpectedCharacterMessage(char character)
{
    std::string errorMsg = "Unexpected character '";
    errorMsg.push_back(character);
    errorMsg += "' at line " + std::to_string(m_lineNumber);

    return errorMsg;
}


ikgen::Result<char, std::string> JsonReader::skipWhitespaceAndReadChar(BufferedFile& file, std::optional<char>& nextCharacter)
{
    char character = '\0';

    while(character == ' ' || character == '\n'
          || character == '\r' || character == '\t' || character == '\0')
    {
        character = readChar(file, nextCharacter);
        if(character == '\n')
            ++m_lineNumber;
    }

    if(character == '\0')
        return ikgen::Result<char, std::string>::makeFailure("Unexpected end of file");

    return ikgen::Result<char, std::string>::success(std::move(character));
}

ReadValueResult JsonReader::readString(BufferedFile& file, std::optional<char>& nextCharacter)
{
    std::string result;

    bool isEscaped = false;
    bool isEscapedUnicode = false;
    unsigned int unicodeEscapeCount = 0;

    char c = readChar(file, nextCharacter);
    if(c != '"')
        return ReadValueResult::failure(buildUnexpectedCharacterMessage(c));

    while(c != '\0')
    {
        c = file.nextChar();

        if(c == '\n')
            ++m_lineNumber;

        if(c == '"' && !isEscaped)
            break;

        if(static_cast<unsigned char>(c) < 0x20)
            return ReadValueResult::failure(buildUnexpectedCharacterMessage(c));

        if(isEscapedUnicode)
        {
            if(c < '0' || (c > '9' && c < 'A') || (c > 'F' && c < 'a') || c > 'f')
                return ReadValueResult::failure(buildUnexpectedCharacterMessage(c));

            ++unicodeEscapeCount;
            if(unicodeEscapeCount == 4)
            {
                unicodeEscapeCount = 0;
                isEscapedUnicode = false;
            }
        }
        else if(isEscaped && (c == '"' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' || c == 'u'))
        {
            isEscaped = false;
            if(c == 'u')
                isEscapedUnicode = true;
        }
        else if(c == '\\')
            isEscaped = !isEscaped;

        result.push_back(c);
    }

    if(c == '\0')
        return ReadValueResult::failure("Unexpected end of file");

    return ReadValueResult::success(std::move(result));
}

ReadValueResult JsonReader::readBasicValue(BufferedFile& file, std::optional<char>& nextCharacter, bool inArray)
{
    char c = readChar(file, nextCharacter);

    if(c == '\n')
        ++m_lineNumber;

    std::string result;

    while(c != '\0' && c != ',' && c != '}' && c != ']')
    {
        result.push_back(c);
        c = file.nextChar();

        if(c == '\n')
            ++m_lineNumber;
    }

    if(c == '\0')
        return ReadValueResult::makeFailure("Unexpected end of file");

    if(c == ']' && !inArray)
        return ReadValueResult::makeFailure(buildUnexpectedCharacterMessage(']'));

    if(c == '}' && inArray)
        return ReadValueResult::makeFailure(buildUnexpectedCharacterMessage('}'));

    nextCharacter = c;

    return ReadValueResult::success(std::move(result));
}

char JsonReader::readChar(BufferedFile& file, std::optional<char>& nextCharacter)
{
    if(nextCharacter.has_value())
    {
        char result = nextCharacter.value();
        nextCharacter.reset();
        return result;
    }
    else
        return file.nextChar();
}

}
