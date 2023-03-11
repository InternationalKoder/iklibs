/*
    Copyright (C) 2023, InternationalKoder

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

#ifndef IKCONF_WARNING_HPP
#define IKCONF_WARNING_HPP

#include "ikconf_export.hpp"
#include <string>

namespace ikconf
{

class Warning
{
    public:

        enum class Type
        {
            SKIPPED_UNKNOWN_PROPERTY
        };

        IKCONF_EXPORT Warning(Type type, std::string message) :
            m_type(type),
            m_message(std::move(message))
        {}

        inline Type getType() const { return m_type; }
        inline std::string_view getMessage() const { return m_message; }

    private:

        Type m_type;
        std::string m_message;
};

}

#endif // IKCONF_WARNING_HPP
