#include "ikconf/readers/JsonReader.hpp"

#include <fstream>
#include "ikconf/exceptions/ConfigurationException.hpp"

namespace ikconf
{
    iklog::Log JsonReader::m_log("JsonReader", iklog::Level::WARNING);

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
            // read array
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
                m_log.warn("Unknown property '" + propertyName + "' was skipped");
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

        while(file >> character && (character == '\n' || character == '\r' || character == '\t' || character == '\0'))
        {
            if(character == '\n')
                m_lineNumber++;
        }

        if(character == '\0' && !acceptEof)
            throw ConfigurationException("Unexpected end of file");

        return character;
    }
}
