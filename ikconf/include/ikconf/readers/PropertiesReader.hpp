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

#ifndef IKCONF_PROPERTIES_READER_HPP
#define IKCONF_PROPERTIES_READER_HPP

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
     * \brief Reader for .properties configuration files
     */
    class PropertiesReader : public BaseReader
    {
        public:

            /*!
             * \brief Constructor which takes the configuration to valorize
             * \param configuration The configuration that will hold the read values
             */
            IKCONF_EXPORT PropertiesReader(const Configuration& configuration);


            /*!
             * \brief Reads the given .properties file and sets the properties in the configuration (given in the constructor)
             * \param filePath Path to the .properties file to read
             */
            IKCONF_EXPORT virtual void read(const std::string& filePath);

        private:

#ifdef IKCONF_USE_IKLOG
            static iklog::Log LOG;
#endif


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
    };
}

#endif // IKCONF_PROPERTIES_READER_HPP
