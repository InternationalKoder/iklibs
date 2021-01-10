/*
    Copyright (C) 2019, 2021, InternationalKoder

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

#include "ikconf/readers/BaseReader.hpp"

namespace ikconf
{
    BaseReader::BaseReader(const Configuration& configuration) :
        m_configuration(configuration)
    {}

    BaseReader::~BaseReader()
    {}

    // specific template for bool, since it can be given as an integer or as a string
    // further interpretation is needed
    template<>
    bool BaseReader::tryConvertAndSetProperty<bool>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        const std::any propertyValue = configuration.getPropertyValue(name);
        const bool basicType = (propertyValue.type() == typeid(bool*));

        if(basicType || propertyValue.type() == typeid(std::vector<bool>*))
        {
            // convert from string to int
            int convertedValue;

            std::string stringValue = std::any_cast<std::string>(value);
            std::from_chars_result conversionResult =
                    std::from_chars(stringValue.data(),stringValue.data() + stringValue.size(), convertedValue);

            // if the conversion fails, interpret the value as a string
            if(conversionResult.ec == std::errc::invalid_argument
                    || conversionResult.ec == std::errc::result_out_of_range)
            {
                std::transform(stringValue.begin(), stringValue.end(), stringValue.begin(),
                        [](char c) { return std::tolower(c); });
                if(basicType)
                    *std::any_cast<bool*>(propertyValue) = (stringValue == TRUE_STR);
                else
                    std::any_cast<std::vector<bool>*>(propertyValue)->push_back(stringValue == TRUE_STR);
                return true;
            }

            // otherwise, interpret the resulting int
            if(basicType)
                *std::any_cast<bool*>(propertyValue) = (convertedValue == TRUE_INT);
            else
                std::any_cast<std::vector<bool>*>(propertyValue)->push_back(convertedValue == TRUE_INT);
            return true;
        }

        return false;
    }

    // specific template for string, no conversion is needed
    template<>
    bool BaseReader::tryConvertAndSetProperty<std::string>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        const std::any propertyValue = configuration.getPropertyValue(name);

        if(propertyValue.type() == typeid(std::string*))
        {
            *std::any_cast<std::string*>(propertyValue) = std::any_cast<std::string>(value);
            return true;
        }
        else if(propertyValue.type() == typeid(std::vector<std::string>*))
        {
            std::any_cast<std::vector<std::string>*>(propertyValue)->push_back(std::any_cast<std::string>(value));
            return true;
        }

        return false;
    }

#ifdef USE_FROM_CHARS_CONVERSION
    // The std::from_chars conversion method doesn't work when we want to convert to a single char,
    // so we have to define our own conversion method for this case

    template<>
    inline bool BaseReader::tryConvertAndSetProperty<char>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        return tryConvertAndSetChar<char>(name, value, configuration);
    }

    template<>
    inline bool BaseReader::tryConvertAndSetProperty<unsigned char>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        return tryConvertAndSetChar<unsigned char>(name, value, configuration);
    }
#endif

    bool BaseReader::tryConvertAndSetProperty(const std::string& name, const std::any& value, Configuration& configuration)
    {
        bool added = false;

        added = tryConvertAndSetProperty<std::string>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<bool>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<float>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<double>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<short int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned short int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<long long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned long long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<char>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned char>(name, value, configuration);
        if(added)
            return true;

        return tryConvertAndSetProperty<long double>(name, value, configuration);
    }

    std::string BaseReader::trim(const std::string& str)
    {
        auto wsfront = std::find_if_not(str.begin(), str.end(), [](char c) { return std::isspace(c); });
        auto wsback = std::find_if_not(str.rbegin(), str.rend(), [](char c) { return std::isspace(c); }).base();
        return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }
}
