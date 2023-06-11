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

#ifndef IKNET_SYSTEMSPEC_HPP
#define IKNET_SYSTEMSPEC_HPP

#include <string>
#include "iknet_export.hpp"

#ifdef _WIN32

#define NOGDI

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <WinSock2.h>
#endif

namespace iknet
{

#ifdef _WIN32
using SocketImpl = SOCKET;
using IoSize = int;

static constexpr SocketImpl DEFAULT_INVALID_SOCKET = INVALID_SOCKET;
static constexpr int DEFAULT_SOCKET_ERROR = SOCKET_ERROR;
#else
using SocketImpl = int;
using IoSize = ssize_t;

static constexpr SocketImpl DEFAULT_INVALID_SOCKET = -1;
static constexpr int DEFAULT_SOCKET_ERROR = -1;
#endif

/*!
 * \brief Gives an error string matching an error code given by system network API
 * \param errorCode The error code given by system network API
 * \return A string describing the error
 */
IKNET_EXPORT std::string formatNetworkError(int errorCode);

/*!
 * \brief Gives an error string matching the last error encountered by system network API
 * \return A string describing the error
 */
IKNET_EXPORT std::string lastNetworkErrorString();

/*!
 * \brief Tells whether the socket is invalid
 * \param socket The socket to check
 * \return True if the socket is invalid
 */
IKNET_EXPORT bool isSocketInvalid(const SocketImpl& socket);

/*!
 * \brief Tells whether the send call returned an error
 * \param sendResult The result of the send call
 * \return True if the code returned by send call is an error
 */
IKNET_EXPORT bool isSendError(IoSize sendResult);

/*!
 * \brief Stops communications on the socket
 * \param socket The socket to shutdown
 */
IKNET_EXPORT void shutdownSocket(SocketImpl& socket);

/*!
 * \brief Closes the socket
 * \param socket The socket to close
 */
IKNET_EXPORT void closeSocket(SocketImpl& socket);

}

#endif // IKNET_SYSTEMSPEC_HPP
