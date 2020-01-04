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

#ifndef IKLOGCONF_OUTPUT_CONFIGURATION_ITEM_HPP
#define IKLOGCONF_OUTPUT_CONFIGURATION_ITEM_HPP

#include "LogConfigurationOutput.hpp"

namespace iklogconf
{
    class OutputConfigurationItem : public LogConfigurationOutput
    {
        public:

            OutputConfigurationItem();

            inline std::string getName() const { return m_name; }

        private:

            std::string m_name; // the name of the output, to reference it
    };
}

#endif // IKLOGCONF_OUTPUT_CONFIGURATION_ITEM_HPP
