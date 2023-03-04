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

#include "ikconf/OtherSubSettings.hpp"

// define the properties in the constructor, like any ikconf configuration
OtherSubSettings::OtherSubSettings() :
    ikconf::Configuration(ikconf::Property("testOtherSubsettingsStr", m_testOtherSubSettingsStr))
{}
