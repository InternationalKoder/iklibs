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

#ifndef IKNET_SOCKET_HPP
#define IKNET_SOCKET_HPP

#include "iknet_export.hpp"
#include "Result.hpp"
#include "iknet/systemspec.hpp"

namespace iknet
{

/*!
 * \brief Base class for a socket of any type
 */
class Socket
{
    public:

        IKNET_EXPORT virtual ~Socket();

        Socket(const Socket& other) = delete;
        Socket(Socket&& other) = delete;


        /*!
         * \brief Makes the socket blocking or not blocking
         * \param blocking Whether the socket should be in blocking mode
         * \return Nothing in case of success, an error message otherwise
         */
        IKNET_EXPORT Result<EmptyResult, std::string> setBlocking(bool blocking);

        inline SocketImpl& getImpl() { return m_socketImpl; }
        inline const SocketImpl& getImpl() const { return m_socketImpl; }

    protected:

        IKNET_EXPORT Socket();
        IKNET_EXPORT explicit Socket(SocketImpl socketImpl);

        SocketImpl m_socketImpl;
};

}

#endif // IKNET_SOCKET_HPP
