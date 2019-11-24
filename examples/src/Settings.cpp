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

#include "Settings.hpp"

// you can define the properties by giving them in the ikconf::Configuration constructor
// the link between the name of the property in the file and the class member is done through the ikconf::Property class
Settings::Settings() :
    ikconf::Configuration(ikconf::Property("my.string", m_testString),
                          ikconf::Property("myint", m_testInt),
                          ikconf::Property("myfloat", m_testFloat),
                          ikconf::Property("mybool", m_testBool),
                          ikconf::Property("mychar.char", m_testCharAsChar),
                          ikconf::Property("mysubsettings", m_testSubSettings),
                          ikconf::Property("othersubsettings", m_testOtherSubSettings),
                          ikconf::Property("myarray", m_testArray),
                          ikconf::Property("mystringarray", m_testStringArray))
{
    // alternatively, you can also define the properties by calling the addProperty and addProperties methods:

    //addProperty(ikconf::Property("my.string", m_testString));
    //addProperties(ikconf::Property("my.string", m_testString), ikconf::Property("myint", m_testInt));
}
