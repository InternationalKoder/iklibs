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
    /*!
     * \brief Wrapper to allow iklog using any std::ostream instance
     */
    class OstreamWrapper : public Output
    {
        public:

            IKLOG_EXPORT static OstreamWrapper COUT; //! wrapper for std::cout
            IKLOG_EXPORT static OstreamWrapper CERR; //! wrapper for std::cerr
            IKLOG_EXPORT static OstreamWrapper CLOG; //! wrapper for std::clog

            IKLOG_EXPORT OstreamWrapper(std::ostream& ostream);

            /*!
             * \brief Writes the given message on the wrapped std::ostream
             * \param message The message to write
             * \return The wrapped stream on which the message has been written
             */
            virtual inline std::ostream& write(const std::string& message) { return *m_ostream << message; }

        private:

            std::ostream* m_ostream;
    };
}

#endif // IKLOG_OSTREAM_WRAPPER_HPP
