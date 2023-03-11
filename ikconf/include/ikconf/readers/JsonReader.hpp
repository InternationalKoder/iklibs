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

#ifndef IKCONF_JSON_READER_HPP
#define IKCONF_JSON_READER_HPP

#include "BaseReader.hpp"

namespace ikconf
{

class BufferedFile;

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
         * \return The warnings that may have been raised while reading the properties, or an error message
         */
        IKCONF_EXPORT virtual ikgen::Result<std::vector<Warning>, std::string> read(const std::string& filePath);

    private:

        /*!
         * \brief Reads a JSON object and sets the properties values it contains
         * \param file The file to read from
         * \param configuration The configuration which contains the properties to set
         * \return The warnings that may have been raised while reading the JSON object, or an error message
         */
        ikgen::Result<std::vector<Warning>, std::string> readObject(BufferedFile& file, Configuration& configuration);


        /*!
         * \brief Reads a JSON array and sets the matching property value
         * \param file The file to read from
         * \param configuration The configuration which contains the properties to set
         * \param propertyName The name of the property which will contain the array's value, the property must be std::vector
         * \return An error message if the reading fails
         */
        ikgen::Result<ikgen::EmptyResult, std::string> readArray(BufferedFile& file, Configuration& configuration, const std::string& propertyName);


        /*!
         * \brief Builds a message indicating that an unexpected character was found during the reading
         * \param character The unexpected character to report
         */
        std::string buildUnexpectedCharacterMessage(char character);


        /*!
         * \brief Reads a character from the given file, ignoring all the blank characters and incrementing line number
         * \param file The file to read from
         * \param acceptEof Tells whether an error should be returned if EOF is met (false = error)
         * \return The read character, or an error message
         */
        ikgen::Result<char, std::string> readCharacter(BufferedFile& file, bool acceptEof = false);


        unsigned int m_lineNumber; // Counter holding the current line number to give when an error occurs
};

}

#endif // IKCONF_JSON_READER_HPP
