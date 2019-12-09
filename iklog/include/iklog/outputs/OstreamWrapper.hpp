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

#ifndef IKLOG_OSTREAM_WRAPPER_HPP
#define IKLOG_OSTREAM_WRAPPER_HPP

#include "Output.hpp"

namespace iklog
{
    class OstreamWrapper : public Output
    {
        public:

            IKLOG_EXPORT static OstreamWrapper COUT;
            IKLOG_EXPORT static OstreamWrapper CERR;
            IKLOG_EXPORT static OstreamWrapper CLOG;

            IKLOG_EXPORT OstreamWrapper(std::ostream& ostream);

            virtual inline std::ostream& write(const std::string& message) { return *m_ostream << message; }

        private:

            std::ostream* m_ostream;
    };
}

#endif // IKLOG_OSTREAM_WRAPPER_HPP
