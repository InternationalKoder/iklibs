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

#include "iklogconf/config-model/LogConfigurationOutput.hpp"

namespace iklogconf
{
    LogConfigurationOutput::LogConfigurationOutput() :
        ikconf::Configuration(ikconf::Property("type", m_type),
                              ikconf::Property("baseFileName", m_baseFileName),
                              ikconf::Property("maxFileSize", m_maxFileSize),
                              ikconf::Property("maxRollingFiles", m_maxRollingFiles))
    {}
}
