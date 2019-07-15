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

    template<>
    bool BaseReader::tryConvertAndAddToPropertyArray<bool>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        const std::any propertyValue = configuration.getPropertyValue(name);

        if(propertyValue.type() == typeid(std::vector<bool>*))
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
                std::any_cast<std::vector<bool>*>(propertyValue)->push_back(stringValue == TRUE_STR);
                return true;
            }

            // otherwise, interpret the resulting int
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

        return false;
    }

    template<>
    bool BaseReader::tryConvertAndAddToPropertyArray<std::string>(const std::string& name, const std::any& value, Configuration& configuration)
    {
        const std::any propertyValue = configuration.getPropertyValue(name);

        if(propertyValue.type() == typeid(std::vector<std::string>*))
        {
            std::any_cast<std::vector<std::string>*>(propertyValue)->push_back(std::any_cast<std::string>(value));
            return true;
        }

        return false;
    }

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

    bool BaseReader::tryConvertAndAddToPropertyArray(const std::string& name, const std::any& value, Configuration& configuration)
    {
        bool added = false;

        added = tryConvertAndAddToPropertyArray<std::string>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<bool>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<unsigned int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<float>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<double>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<short int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<unsigned short int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<unsigned long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<long long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<unsigned long long int>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<char>(name, value, configuration);
        if(added)
            return true;

        added = tryConvertAndAddToPropertyArray<unsigned char>(name, value, configuration);
        if(added)
            return true;

        return tryConvertAndAddToPropertyArray<long double>(name, value, configuration);
    }

    std::string BaseReader::trim(const std::string& str)
    {
        auto wsfront = std::find_if_not(str.begin(), str.end(), [](char c) { return std::isspace(c); });
        auto wsback = std::find_if_not(str.rbegin(), str.rend(), [](char c) { return std::isspace(c); }).base();
        return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }
}
