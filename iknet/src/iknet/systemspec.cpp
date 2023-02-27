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

#include "iknet/systemspec.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

namespace iknet
{

std::string formatNetworkError(int errorCode)
{
#ifdef _WIN32
    LPTSTR errorMessage = nullptr;

    DWORD formatResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                                       nullptr,
                                       errorCode,
                                       0,
                                       errorMessage,
                                       0,
                                       nullptr);

    if(formatResult <= 0 || errorMessage == nullptr)
        return "Error code " + std::to_string(errorCode);

    return errorMessage;
#else
    return std::string(strerror(errorCode));
#endif
}

std::string lastNetworkErrorString()
{
#ifdef _WIN32
    return formatNetworkError(WSAGetLastError());
#else
    return formatNetworkError(errno);
#endif
}

bool isSocketInvalid(const SocketImpl& socket)
{
#ifdef _WIN32
    return socket == INVALID_SOCKET;
#else
    return socket < 0;
#endif
}

bool isSendError(int sendResult)
{
#ifdef _WIN32
    return sendResult == SOCKET_ERROR;
#else
    return sendResult < 0;
#endif
}

void shutdownSocket(SocketImpl& socket)
{
#ifdef _WIN32
    shutdown(socket, SD_SEND);
#else
    shutdown(socket, SHUT_WR);
#endif
}

void closeSocket(SocketImpl& socket)
{
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

}
