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

#ifndef IKCONF_BASE_READER_HPP
#define IKCONF_BASE_READER_HPP

#include "ikconf/Configuration.hpp"
#include "ikconf/Warning.hpp"
#include <ikgen/Result.hpp>
#include <charconv>
#include <algorithm>

#if _MSC_VER >= 1924
#define USE_FROM_CHARS_CONVERSION
#endif

#ifndef USE_FROM_CHARS_CONVERSION
#include <sstream>
#endif

namespace ikconf
{

/*!
 * \brief Base class for the configuration readers
 *
 * Configuration readers are the classes that handle the reading of a configuration file and the filling of the
 * values in the configuration class
 */
class BaseReader
{
    public:

        /*!
         * \brief Reads the given file and sets the properties in the configuration (given in the constructor)
         * \param filePath Path to the file to read
         * \param configuration The configuration that will hold the read values
         * \return The warnings that may have been raised while reading the properties, or an error message
         */
        virtual ikgen::Result<std::vector<Warning>, std::string> read(const std::string& filePath, Configuration& configuration) = 0;

    protected:

        /*!
         * \brief Tries to convert the given property, and sets the value in the configuration if possible
         * \param name The name of the property that will receive the converted value
         * \param value The value to convert and to set
         * \param configuration The (sub)configuration containing the property to set
         * \return True if the conversion and setting have been successfully done
         */
        static bool tryConvertAndSetProperty(const std::string& name, const std::any& value, Configuration& configuration);


        /*!
         * \brief Generic template for all the fundamental types, except bool
         */
        template<typename T>
        static bool tryConvertAndSetProperty(const std::string& name, const std::any& value, Configuration& configuration)
        {
            const std::any propertyValue = configuration.getPropertyValue(name);
            const bool basicType = (propertyValue.type() == typeid(T*));

            if(basicType || propertyValue.type() == typeid(std::vector<T>*))
            {
                T convertedValue;


                // convert from string to T
                const std::string stringValue = std::any_cast<std::string>(value);
#ifdef USE_FROM_CHARS_CONVERSION
                std::from_chars_result conversionResult =
                        std::from_chars(stringValue.data(), stringValue.data() + stringValue.size(), convertedValue);

                // stop if the conversion failed
                if(conversionResult.ec == std::errc::invalid_argument
                        || conversionResult.ec == std::errc::result_out_of_range)
                {
                    return false;
                }
#else
                std::istringstream conversionStream(stringValue);

                // stop if the conversion failed
                if(!(conversionStream >> convertedValue))
                {
                    return false;
                }
#endif

                // set the value in the property
                if(basicType)
                    *std::any_cast<T*>(propertyValue) = convertedValue;
                else
                    std::any_cast<std::vector<T>*>(propertyValue)->push_back(convertedValue);

                return true;
            }

            return false;
        }


        /*!
         * \brief Removes the blank characters at the beginning and at the end of a std::string
         * \param string The string to trim
         * \return The trimmed string
         */
        static std::string trim(const std::string& string);

    private:

#ifdef USE_FROM_CHARS_CONVERSION
        /*!
         * The std::from_chars conversion method doesn't work when we want to convert to a single char,
         * so we have to define our own conversion method for this case
         */
        template<typename T,
                 std::enable_if_t<std::is_same_v<T, char> ||
                                  std::is_same_v<T, unsigned char>>* = nullptr>
        static bool tryConvertAndSetChar(const std::string& name, const std::any& value, Configuration& configuration)
        {
            const std::any propertyValue = configuration.getPropertyValue(name);

            if(propertyValue.type() == typeid(T*))
            {
                const std::string stringValue = std::any_cast<std::string>(value);

                if(!stringValue.empty())
                {
                    *std::any_cast<T*>(propertyValue) = stringValue[0];
                    return true;
                }
            }

            return false;
        }
#endif

        // values to interpret as the 'true' boolean, other values are considered as 'false'
        static constexpr std::string_view TRUE_STR = "true";
        static constexpr int TRUE_INT = 1;
};

}

#endif // IKCONF_BASE_READER_HPP
