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

#ifndef IKLOG_OUTPUT_HPP
#define IKLOG_OUTPUT_HPP

#include <string>
#include <ostream>
#include "../iklog_export.hpp"

namespace iklog
{
    /*!
     * \brief Base class for iklog outputs
     *
     * Abstract class that defines the base of a iklog output system
     */
    class Output
    {
        public:

            IKLOG_EXPORT virtual ~Output();

            /*!
             * \brief Writes the given string to the output
             * \param message The string to write
             * \return A stream stream on which the message has been written
             */
            virtual std::ostream& write(const std::string& message) = 0;
    };

    std::ostream& operator<<(Output& output, const std::string& message);
}

#endif // IKLOG_OUTPUT_HPP
