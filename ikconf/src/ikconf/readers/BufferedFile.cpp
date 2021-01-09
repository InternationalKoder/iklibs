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

#include "ikconf/readers/BufferedFile.hpp"
#include <algorithm>

namespace ikconf
{
    BufferedFile::BufferedFile(const std::string& filePath) :
        m_file(filePath.c_str()),
        m_buffer(BUFFER_SIZE),
        m_bufferPos(0),
        m_bufferSize(0)
    {}

    char BufferedFile::nextChar()
    {
        // fill buffer if we have already read it all
        if(m_bufferPos >= m_bufferSize)
        {
            fillBuffer();
            if(m_bufferPos >= m_bufferSize)
                return '\0';
        }

        // return character from buffer
        return m_buffer[m_bufferPos++];
    }

    std::string BufferedFile::nextLine()
    {
        // fill buffer if we have already read it all
        if(m_bufferPos >= m_bufferSize)
        {
            fillBuffer();
            if(m_bufferPos >= m_bufferSize)
                return "";
        }

        // return line from buffer
        bool fullLine = false;
        std::string line = "";
        while(!fullLine && m_bufferPos < m_bufferSize)
        {
            const auto beginIt = m_buffer.begin() + m_bufferPos;
            const auto endIt = m_buffer.begin() + m_bufferSize;
            auto lineEnd = std::find(beginIt, m_buffer.begin() + m_bufferSize, '\n');

            if(lineEnd == beginIt)
            {
                if(*beginIt == '\n')
                {
                    ++m_bufferPos;
                    return line;
                }
            }

            std::string linePart(beginIt, lineEnd);
            line += linePart;
            m_bufferPos += linePart.size() + 1; // + 1 is for the dropped '\n'

            // if the "end of line" character has not been found, we have to fill the buffer again
            if(lineEnd == endIt)
                fillBuffer();
            else
                fullLine = true;
        }

        return line;
    }

    void BufferedFile::fillBuffer()
    {
        // nothing to do if there is nothing to read from the file
        if(!m_file.is_open() || m_file.eof())
            return;

        // fill buffer
        m_buffer.clear();
        m_file.read(m_buffer.data(), BUFFER_SIZE);
        m_bufferPos = 0;
        m_bufferSize = m_file.gcount();
    }
}
