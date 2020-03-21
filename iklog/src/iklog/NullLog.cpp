/*
    Copyright (C) 2020, InternationalKoder

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

#include "iklog/NullLog.hpp"
#include "iklog/Log.hpp"

namespace iklog
{
    NullLog NullLog::m_nullLog;

    NullLog::NullLog() :
        Log("__null_log__", 0, DEFAULT_OUTPUT, Formatter(""))
    {}
}
