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

#include "iknet/SocketInitializer.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#include <stdexcept>
#endif

namespace iknet::priv
{

SocketInitializer::SocketInitializer()
{
#ifdef _WIN32
    WSAData wsaData;

    const int initResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(initResult != 0)
        throw std::runtime_error("Failed to initialize Winsock 2 API");
#endif
}

SocketInitializer::~SocketInitializer()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

}
