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

#ifndef IKLOG_ROLLING_FILE_OUTPUT_HPP
#define IKLOG_ROLLING_FILE_OUTPUT_HPP

#include "Output.hpp"
#include "iklog/files/FileSize.hpp"
#include <ikgen/Result.hpp>
#include <fstream>
#include <string>
#include <assert.h>

namespace iklog
{

/*!
 * \brief Outputs logging messages to a file, with a rolling system
 *
 * When started, this output system creates a new file, for example "myfile.log.0" (".0" is added by this class).
 * Then, when the maximum file size is reached for this file, a rolling is performed:
 * - File "myfile.log.0" is renamed to "myfile.log.1"
 * - A new empty file "myfile.log.0" is created, and new logs will be written in this one
 * When the maximum number of files is reached, the oldest file is removed to save disk space
 */
class RollingFileOutput : public Output
{
    public:

        /*!
         * \brief Creates a new instance of RollingFileOutput. Same as constructors but returns a Result
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxFileSize The maximum size for each logging file, a new file is created when reached
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         * \return Either the newly created RollingFileOutput in case of success, or an error message otherwise
         */
        template<unsigned short MULTIPLIER>
        static ikgen::Result<RollingFileOutput, std::string> create(const std::string& baseFilename, const FileSize<MULTIPLIER>& maxFileSize,
                                                                    unsigned int maxRollingFiles)
        {
            try
            {
                return ikgen::Result<RollingFileOutput, std::string>::makeSuccess(baseFilename, maxFileSize, maxRollingFiles);
            }
            catch(const std::runtime_error& e)
            {
                return ikgen::Result<RollingFileOutput, std::string>::makeFailure(e.what());
            }
        }

        /*!
         * \brief Constructor, throws std::runtime_error if a problem occurs
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxFileSize The maximum size in bytes for each logging file, a new file is created when reached
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         */
        IKLOG_EXPORT RollingFileOutput(const std::string& baseFilename, const Bytes& maxFileSize,
                                       unsigned int maxRollingFiles);

        /*!
         * \brief Constructor, throws std::runtime_error if a problem occurs
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxFileSize The maximum size in kilobytes for each logging file, a new file is created when reached
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         */
        IKLOG_EXPORT RollingFileOutput(const std::string& baseFilename, const KiloBytes& maxFileSize,
                                       unsigned int maxRollingFiles);

        /*!
         * \brief Constructor, throws std::runtime_error if a problem occurs
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxFileSize The maximum size in megabytes for each logging file, a new file is created when reached
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         */
        IKLOG_EXPORT RollingFileOutput(const std::string& baseFilename, const MegaBytes& maxFileSize,
                                       unsigned int maxRollingFiles);

        /*!
         * \brief Constructor, throws std::runtime_error if a problem occurs
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxFileSize The maximum size in gigabytes for each logging file, a new file is created when reached
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         */
        IKLOG_EXPORT RollingFileOutput(const std::string& baseFilename, const GigaBytes& maxFileSize,
                                       unsigned int maxRollingFiles);

        IKLOG_EXPORT virtual ~RollingFileOutput();

        /*!
         * \brief Writes the given message to the current file in the rolling system
         * \param message The message to write
         * \return The stream that was used to write the message
         */
        IKLOG_EXPORT virtual std::ostream& write(const std::string& message);

    private:

        static constexpr char SEPARATOR = '.'; // separator that will go between the file name and the rolling index


        /*!
         * \brief Constructor
         * \param baseFilename The file name without the rolling system extension, for example "myfile.log"
         * \param maxRollingFiles The maximum number of files, the oldest file is removed when reached
         * \param maxFileSize The maximum size for each logging file, a new file is created when reached
         */
        template<unsigned short MULTIPLIER>
        RollingFileOutput(const std::string& baseFilename, unsigned int maxRollingFiles,
                          const FileSize<MULTIPLIER>& maxFileSize) :
            iklog::Output(),
            m_maxRoll(maxRollingFiles - 1),
            m_maxFileSize(maxFileSize.getValueInBytes()),
            m_fileSizeCache(0),
            m_cacheValidityThreshold(m_maxFileSize / 2),
            m_baseFilenameSep(baseFilename + SEPARATOR),
            m_firstRollingFileName(m_baseFilenameSep + '0'),
            m_lastRollingFileName(m_baseFilenameSep + std::to_string(m_maxRoll))
        {
            assert(m_maxFileSize > 0);
            assert(maxRollingFiles > 0);
            m_file.open(m_firstRollingFileName, std::ios::out | std::ios::app);
            if(!m_file.is_open())
                throw std::runtime_error("Failed to open file '" + m_firstRollingFileName + "' in write mode");
        }


        /*!
         * \brief Rolls the files
         *
         * File number 3 becomes file number 4, file number 2 becomes file number 3, etc.
         * File number 0 is a new file
         */
        void roll();


        std::ofstream m_file; // current file we write to
        const unsigned int m_maxRoll; // maximum number of files in the rolling system
        const uintmax_t m_maxFileSize; // maximum file size after which a rolling is performed

        uintmax_t m_fileSizeCache; // estimated file size, to avoid reading the file system at each writing operation
        uintmax_t m_cacheValidityThreshold; // estimated file size at which the cache is invalidated and the actual file size is read again

        const std::string m_baseFilenameSep; // base file name + the separator character
        const std::string m_firstRollingFileName; // actual file name of the first file (the one we write to)
        const std::string m_lastRollingFileName; // actual file name of the last file (the next one to be removed)
};

}

#endif // IKLOG_ROLLING_FILE_OUTPUT_HPP
