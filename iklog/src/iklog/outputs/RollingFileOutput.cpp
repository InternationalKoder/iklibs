/*
    Copyright (C) 2019, 2023, InternationalKoder

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

#include "iklog/outputs/RollingFileOutput.hpp"
#include <filesystem>
#include <string>

namespace iklog
{

RollingFileOutput::RollingFileOutput(const std::string& baseFilename, const Bytes& maxFileSize,
                                     unsigned int maxRollingFiles) :
    RollingFileOutput(baseFilename, maxRollingFiles, maxFileSize)
{}

RollingFileOutput::RollingFileOutput(const std::string& baseFilename, const KiloBytes& maxFileSize,
                                     unsigned int maxRollingFiles) :
    RollingFileOutput(baseFilename, maxRollingFiles, maxFileSize)
{}

RollingFileOutput::RollingFileOutput(const std::string& baseFilename, const MegaBytes& maxFileSize,
                                     unsigned int maxRollingFiles) :
    RollingFileOutput(baseFilename, maxRollingFiles, maxFileSize)
{}

RollingFileOutput::RollingFileOutput(const std::string& baseFilename, const GigaBytes& maxFileSize,
                                     unsigned int maxRollingFiles) :
    RollingFileOutput(baseFilename, maxRollingFiles, maxFileSize)
{}

RollingFileOutput::~RollingFileOutput() = default;

std::ostream& RollingFileOutput::write(const std::string& message)
{
    // update cache with estimated file size (+ line ending)
    m_fileSizeCache += message.length() + 2;

    // if necessary, update cache with actual file size
    if(m_fileSizeCache > m_cacheValidityThreshold)
    {
        m_fileSizeCache = std::filesystem::file_size(m_firstRollingFileName);
        m_cacheValidityThreshold = (m_fileSizeCache + m_maxFileSize) / 2;
    }

    // if the file is too large, do the rolling
    if(m_fileSizeCache >= m_maxFileSize)
        roll();

    return m_file << message;
}

void RollingFileOutput::roll()
{
    // if the last file of the rolling system exists, then we have to remove it
    if(std::filesystem::exists(m_lastRollingFileName))
        std::filesystem::remove(m_lastRollingFileName);

    m_file.close();

    // roll the files if they exist
    for(int roll = static_cast<int>(m_maxRoll) - 1 ; roll >= 0 ; roll--)
    {
        const std::string fileToRoll = m_baseFilenameSep + std::to_string(roll);

        if(std::filesystem::exists(fileToRoll))
            std::filesystem::rename(fileToRoll, m_baseFilenameSep + std::to_string(roll + 1));
    }

    // reset cache
    m_fileSizeCache = 0;
    m_cacheValidityThreshold = 0;

    // open new file
    m_file.open(m_firstRollingFileName);
}

}
