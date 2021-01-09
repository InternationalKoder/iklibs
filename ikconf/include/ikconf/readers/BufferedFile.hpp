/*
    Copyright (C) 2021, InternationalKoder

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

#ifndef IKCONF_BUFFERED_FILE_HPP
#define IKCONF_BUFFERED_FILE_HPP

#include <fstream>
#include <vector>

namespace ikconf
{
    /*!
     * \brief Reads a file using a buffer
     */
    class BufferedFile
    {
        public:

            /*!
             * \brief Constructor opening the file to read
             * \param filePath Path to the file to read
             */
            BufferedFile(const std::string& filePath);

            /*!
             * \brief Reads a char from the file
             * \return The char that has been read from the file
             */
            char nextChar();

            /*!
             * \brief Reads a line from the file
             * \return The line that has been read from the file
             */
            std::string nextLine();

            /*!
             * \brief Checks if the file is actually open
             * \return True if the file is open, false otherwise
             */
            inline bool isOpen() const { return m_file.is_open(); }

            /*!
             * \brief Checks if the end of file has been reached
             * \return True if the end of file has been reached, false otherwise
             */
            inline bool isEof() const { return m_bufferPos >= m_bufferSize && m_file.eof(); }

        private:

            static constexpr size_t BUFFER_SIZE = 1048576; // Buffer size in bytes


            /*!
             * \brief Actually read the file and fill the buffer
             */
            void fillBuffer();


            std::ifstream m_file; // File to read
            std::vector<char> m_buffer; // File reading buffer
            size_t m_bufferPos; // Position in buffer
            size_t m_bufferSize; // Actual number of bytes coming from the file in the buffer
    };
}

#endif // IKCONF_BUFFERED_FILE_HPP
