/*
    Copyright (C) 2019, 2020, InternationalKoder

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

#ifndef IKCONF_PROPERTY_HPP
#define IKCONF_PROPERTY_HPP

#include <string>
#include <vector>
#include <any>

namespace ikconf
{
    /*!
     * \brief A property: name and value
     *
     * Holds only a reference to the given value
     */
    template<typename T>
    class Property
    {
        public:

            /*!
             * \brief Constructor initializing the property
             * \param name The name of the property
             * \param value The value of the property, must be a lvalue
             */
            Property(const std::string& name, T& value) : m_name(name), m_value(&value) {}


            inline const std::string& getName() const { return m_name; }
            inline T* getValue() const { return m_value; }

        private:

            const std::string m_name;
            T* const m_value;
    };
}

#endif // IKCONF_PROPERTY_HPP
