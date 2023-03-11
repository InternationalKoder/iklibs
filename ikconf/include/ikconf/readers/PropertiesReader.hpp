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

#ifndef IKCONF_PROPERTIES_READER_HPP
#define IKCONF_PROPERTIES_READER_HPP

#include "BaseReader.hpp"

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
         * \return The warnings that may have been raised while reading the properties
         */
        IKCONF_EXPORT virtual std::vector<Warning> read(const std::string& filePath);
};

}

#endif // IKCONF_PROPERTIES_READER_HPP
