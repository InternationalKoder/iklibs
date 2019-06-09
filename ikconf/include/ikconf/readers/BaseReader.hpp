#ifndef BASE_READER_HPP
#define BASE_READER_HPP

#include "ikconf/ikconf_export.hpp"
#include "ikconf/Configuration.hpp"
#include "ikconf/ikconf_export.hpp"
#include <charconv>
#include <algorithm>
#include <sstream>

namespace ikconf
{
    class BaseReader
    {
        public:

            BaseReader(const Configuration& configuration);

            IKCONF_EXPORT virtual ~BaseReader();

            virtual void read(const std::string& filePath) = 0;

        protected:

            // generic template for all the fundamental types, except bool
            template<typename T>
            bool tryConvertAndSetProperty(const std::string& name, const std::any& value)
            {
                const std::any propertyValue = m_configuration.getPropertyValue(name);

                if(propertyValue.type() == typeid(T*))
                {
                    T convertedValue;

                    /*
                    not supported by many compilers yet, we have to use the less effective way, based on streams

                    // convert from string to T
                    const std::string stringValue = std::any_cast<std::string>(value);
                    std::from_chars_result conversionResult =
                            std::from_chars(stringValue.data(), stringValue.data() + stringValue.size(), convertedValue);

                    // stop if the conversion failed
                    if(conversionResult.ec == std::errc::invalid_argument
                            || conversionResult.ec == std::errc::result_out_of_range)
                    {
                        return false;
                    }
                    */

                    // convert from string to T
                    const std::string stringValue = std::any_cast<std::string>(value);
                    std::istringstream conversionStream(stringValue);

                    // stop if the conversion failed
                    if(!(conversionStream >> convertedValue))
                    {
                        return false;
                    }

                    // set the value in the property
                    *std::any_cast<T*>(propertyValue) = convertedValue;

                    return true;
                }

                return false;
            }

            // gather implementation of tryConvertAndSetProperty for all the fundamental types and std::string
            bool tryConvertAndSetProperty(const std::string& name, const std::any& value);

            Configuration m_configuration;

        private:

            static constexpr char TRUE_STR[] = "true";
            static constexpr int TRUE_INT = 1;
    };
}

#endif // BASE_READER_HPP
