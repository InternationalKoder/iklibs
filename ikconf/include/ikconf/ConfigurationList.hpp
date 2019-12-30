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

#ifndef IKCONF_CONFIGURATION_LIST_HPP
#define IKCONF_CONFIGURATION_LIST_HPP

#include "Configuration.hpp"
#include <vector>
#include <any>
#include <string>
#include "ikconf_export.hpp"

namespace ikconf
{
    /*!
     * \brief Special configuration item that is actually a list of objects
     */
    template<typename T>
    class ConfigurationList : public Configuration
    {
        public:

            /*!
             * \brief Adds a new item in the list
             * \return A pointer to the new item in the list
             */
            virtual Configuration* newListItem() override
            {
                m_properties.emplace_back();
                return &m_properties.back();
            }

            inline std::vector<T> getProperties() const { return m_properties; }

        private:

            std::vector<T> m_properties;
    };
}

#endif // IKCONF_CONFIGURATION_LIST_HPP
