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

#ifndef SUB_SETTINGS_HPP
#define SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>
#include "SubSubSettings.hpp"

/*!
 * \brief Shows how to implement a subobject in a configuration (ikconf)
 */
class SubSettings : public ikconf::Configuration
{
    public:

        SubSettings();

        inline std::string getTestSubString() const { return m_testSubString; }
        inline int getTestSubInt() const { return m_testSubInt; }
        inline SubSubSettings getTestSubSubSettings() const { return m_testSubSubSettings; }

    private:

        std::string m_testSubString;
        int m_testSubInt;
        SubSubSettings m_testSubSubSettings;
};

#endif // SUB_SETTINGS_HPP
