#include "ikconf/readers/PropertiesReader.hpp"

#include <fstream>
#include "ikconf/exceptions/ConfigurationException.hpp"

namespace ikconf
{
    iklog::Log PropertiesReader::LOG("PropertiesReader", iklog::Level::WARNING);

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
                        LOG.warn("Unknown property '" + propertyName + "' was skipped");
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
