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

#include "iknet/Socket.hpp"
#include <utility>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace iknet
{

using SetBlockingResult = ikgen::Result<ikgen::EmptyResult, std::string>;

Socket::Socket() :
    m_socketImpl()
{}

Socket::Socket(SocketImpl socketImpl) :
    m_socketImpl(std::move(socketImpl))
{}

Socket::~Socket()
{
    iknet::shutdownSocket(m_socketImpl);
    iknet::closeSocket(m_socketImpl);
}

SetBlockingResult Socket::setBlocking(bool blocking)
{
#ifdef _WIN32
    u_long mode = blocking ? 0 : 1;
    const int ctlResult = ioctlsocket(m_socketImpl, FIONBIO, &mode);

    if(ctlResult != NO_ERROR)
        return SetBlockingResult::makeFailure("Failed to change blocking mode of socket: " + lastNetworkErrorString());
#else
    const int prevFdFlags = fcntl(m_socketImpl, F_GETFL, 0);
    if(prevFdFlags < 0)
        return SetBlockingResult::makeFailure("Failed to change blocking mode of socket: " + lastNetworkErrorString());

    long fdFlags = blocking ? (prevFdFlags & ~O_NONBLOCK) : (prevFdFlags | O_NONBLOCK);
    const int fcntlResult = fcntl(m_socketImpl, F_SETFL, fdFlags);

    if(fcntlResult < 0)
        return SetBlockingResult::makeFailure("Failed to change blocking mode of socket: " + lastNetworkErrorString());
#endif

    return SetBlockingResult::makeSuccess();
}

}
