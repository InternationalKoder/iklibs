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

#ifndef IKCONF_CONFIGURATION_EXCEPTION_HPP
#define IKCONF_CONFIGURATION_EXCEPTION_HPP

#include <exception>
#include <string>
#include "ikconf/ikconf_export.hpp"

namespace ikconf
{
    /*!
     * \brief Exception that is thrown when a problem occurs while building a configuration
     */
    class ConfigurationException : public std::exception
    {
        public:

            /*!
             * \brief Constructor which takes the description of the encountered problem
             * \param what Description of the problem
             */
            ConfigurationException(const std::string& what);


            /*!
             * \brief Gives the description of the error
             * \return A string describing the problem
             */
            inline virtual const char* what() const noexcept { return m_what.c_str(); }

        private:

            const std::string m_what;
    };
}

#endif // IKCONF_CONFIGURATION_EXCEPTION_HPP
