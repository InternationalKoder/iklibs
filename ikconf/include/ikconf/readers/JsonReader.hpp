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

#ifndef IKCONF_JSON_READER_HPP
#define IKCONF_JSON_READER_HPP

#include "BaseReader.hpp"
#include "../lib_conf.hpp"

#ifdef IKCONF_USE_IKLOG
#include <iklog/Log.hpp>
#else
#include <iostream>
#endif

namespace ikconf
{
    /*!
     * \brief Reader for JSON configuration files
     *
     * Supports:
     * - objects containing subobjects
     * - arrays, the implementation type must be std::vector
     *
     * Does NOT support:
     * - arrays as the base item, it means the base level must be an object (file must start with '{' and end with '}')
     */
    class JsonReader : public BaseReader
    {
        public:

            /*!
             * \brief Constructor which takes the configuration to valorize
             * \param configuration The configuration that will hold the read values
             */
            IKCONF_EXPORT JsonReader(const Configuration& configuration);

            /*!
             * \brief Reads the given JSON file and sets the properties in the configuration (given in the constructor)
             * \param filePath Path to the JSON file to read
             */
            IKCONF_EXPORT virtual void read(const std::string& filePath);

        private:

#ifdef IKCONF_USE_IKLOG
            static iklog::Log LOG;
#endif

            /*!
             * \brief Reads a JSON object and sets the properties values it contains
             * \param file The file to read from
             * \param configuration The configuration which contains the properties to set
             */
            void readObject(std::ifstream& file, Configuration& configuration);


            /*!
             * \brief Reads a JSON array and sets the matching property value
             * \param file The file to read from
             * \param configuration The configuration which contains the properties to set
             * \param propertyName The name of the property which will contain the array's value, the property must be std::vector
             */
            void readArray(std::ifstream& file, Configuration& configuration, const std::string& propertyName);


            /*!
             * \brief Throws an exception indicating that an unexpected character was found during the reading
             * \param character The unexpected character to report
             */
            [[noreturn]] void handleUnexpectedCharacter(char character);


            /*!
             * \brief Reads a character from the given file, ignoring all the blank characters and incrementing line number
             * \param file The file to read from
             * \param acceptEof Tells whether an exception should be thrown if EOF is met (false = throw)
             * \return The read character
             */
            char readCharacter(std::ifstream& file, bool acceptEof = false);

            /*!
             * \brief Writes a message using iklog if enabled, or standard output otherwise
             * \param message The message to log
             */
            inline void logWarningMessage(const std::string& message)
            {
#ifdef IKCONF_USE_IKLOG
                LOG.warn(message);
#else
                std::cout << "[WARN] " << message << std::endl;
#endif
            }


            unsigned int m_lineNumber; // Counter holding the current line number to give when an error occurs
    };
}

#endif // IKCONF_JSON_READER_HPP
