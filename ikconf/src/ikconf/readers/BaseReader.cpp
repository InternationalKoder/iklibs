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
    bool BaseReader::tryConvertAndSetProperty<bool>(const std::string& name, const std::any& value)
    {
        const std::any propertyValue = m_configuration.getPropertyValue(name);

        if(propertyValue.type() == typeid(bool*))
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
                *std::any_cast<bool*>(propertyValue) = (stringValue == TRUE_STR);
                return true;
            }

            // otherwise, interpret the resulting int
            *std::any_cast<bool*>(propertyValue) = (convertedValue == TRUE_INT);
            return true;
        }

        return false;
    }

    // specific template for string, no conversion is needed
    template<>
    bool BaseReader::tryConvertAndSetProperty<std::string>(const std::string& name, const std::any& value)
    {
        const std::any propertyValue = m_configuration.getPropertyValue(name);

        if(propertyValue.type() == typeid(std::string*))
        {
            *std::any_cast<std::string*>(propertyValue) = std::any_cast<std::string>(value);
            return true;
        }

        return false;
    }

    bool BaseReader::tryConvertAndSetProperty(const std::string& name, const std::any& value)
    {
        bool added = false;

        added = tryConvertAndSetProperty<std::string>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<bool>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<float>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<double>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<short int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned short int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<long int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned long int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<long long int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned long long int>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<char>(name, value);
        if(added)
            return true;

        added = tryConvertAndSetProperty<unsigned char>(name, value);
        if(added)
            return true;

        return tryConvertAndSetProperty<long double>(name, value);
    }
}
