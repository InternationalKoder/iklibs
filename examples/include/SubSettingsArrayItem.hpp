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

#ifndef SUB_SETTINGS_ARRAY_ITEM_HPP
#define SUB_SETTINGS_ARRAY_ITEM_HPP

#include <ikconf/Configuration.hpp>

/*!
 * \brief Shows how to implement an list item (nothing special actually, it's just like the other configuration objects)
 */
class SubSettingsArrayItem : public ikconf::Configuration
{
    public:

        SubSettingsArrayItem();

        inline const std::string& getValue() const { return m_value; }
        inline unsigned int getIndex() const { return m_index; }

    private:

        std::string m_value;
        unsigned int m_index;
};

#endif // SUB_SETTINGS_ARRAY_ITEM_HPP
