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

#ifndef OTHER_SUB_SETTINGS_HPP
#define OTHER_SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>

/*!
 * \brief Allows to test multiple subobjects in a configuration (ikconf)
 */
class OtherSubSettings : public ikconf::Configuration
{
    public:

        OtherSubSettings();

        inline const std::string& getTestOtherSubSettingsStr() const { return m_testOtherSubSettingsStr; }

    private:

        std::string m_testOtherSubSettingsStr;
};

#endif // OTHER_SUB_SETTINGS_HPP
