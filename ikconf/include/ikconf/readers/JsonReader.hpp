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
#include "ikconf/ikconf_export.hpp"

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
 * - arrays of arrays
 * - arrays as the base item, if its elements' type is not ikconf::Configuration
 */
class JsonReader : public BaseReader
{
    public:

        /*!
         * \brief Reads the given JSON file and sets the properties in the configuration (given in the constructor)
         * \param filePath Path to the JSON file to read
         * \param configuration The configuration that will hold the read values
         * \return The warnings that may have been raised while reading the properties, or an error message
         */
        IKCONF_EXPORT virtual ikgen::Result<std::vector<Warning>, std::string> read(const std::string& filePath, Configuration& configuration);

    private:

        /*!
         * \brief Builds a message indicating that an unexpected character was found during the reading
         * \param character The unexpected character to report
         */
        std::string buildUnexpectedCharacterMessage(char character);


        /*!
         * \brief Reads a character from the given file, ignoring all the blank characters and incrementing line number
         * \param file The file to read from
         * \param nextCharacter Holds the optional next character to read instead of reading the file directly
         * \return The read character, or an error message
         */
        ikgen::Result<char, std::string> skipWhitespaceAndReadChar(BufferedFile& file, std::optional<char>& nextCharacter);

        /*!
         * \brief Reads a string (starting and ending with '"') from the given file, ignoring all the blank characters and incrementing line number
         * \param file The file to read from
         * \param nextCharacter Holds the optional next character to read instead of reading the file directly
         * \return The read string, or an error message
         */
        ikgen::Result<std::string, std::string> readString(BufferedFile& file, std::optional<char>& nextCharacter);

        /*!
         * \brief Reads a value of any basic type (int, float, etc.) from the given file
         * \param file The file to read from
         * \param nextCharacter Holds the optional next character to read instead of reading the file directly
         * \param inArray Indicates whether the current node is an array or an object
         * \return The read basic value as a string, or an error message
         */
        ikgen::Result<std::string, std::string> readBasicValue(BufferedFile& file, std::optional<char>& nextCharacter, bool inArray);

        /*!
         * \brief readChar Read a character, either from the nextCharacter buffer if not empty, or from the file
         * \param file The file to read from
         * \param nextCharacter Optional next character to read instead of reading the file directly
         * \return The read character
         */
        char readChar(BufferedFile& file, std::optional<char>& nextCharacter);


        unsigned int m_lineNumber; // Counter holding the current line number to give when an error occurs
};

}

#endif // IKCONF_JSON_READER_HPP
