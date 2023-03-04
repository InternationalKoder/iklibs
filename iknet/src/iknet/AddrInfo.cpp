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

#include "iknet/AddrInfo.hpp"
#include "iknet/systemspec.hpp"
#include <exception>

#ifndef _WIN32
#include <netdb.h>
#include <cstring>
#endif

namespace iknet
{

ikgen::Result<AddrInfo, std::string> AddrInfo::resolve(const std::string& host, uint16_t port, Protocol protocol, bool passive)
{
    try
    {
        return ikgen::Result<AddrInfo, std::string>::makeSuccess(host, port, protocol, passive);
    }
    catch(std::runtime_error& e)
    {
        return ikgen::Result<AddrInfo, std::string>::makeFailure(e.what());
    }
}

AddrInfo::AddrInfo(const std::string& host, uint16_t port, Protocol protocol, bool passive)
{
    const std::pair<int, int>& systemIdentifiers = resolveSystemIdentifiers(protocol);

    addrinfo hints;
#ifdef _WIN32
    ZeroMemory(&hints, sizeof(hints));
#else
    memset(&hints, 0, sizeof(hints));
#endif
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = systemIdentifiers.first;
    hints.ai_protocol = systemIdentifiers.second;

    if(passive)
        hints.ai_flags = AI_PASSIVE;

    addrinfo* info = nullptr;
    const int addrinfoResult = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &info);
    if(addrinfoResult != 0)
        throw std::runtime_error("Failed to resolve address " + host + ": " + formatNetworkError(addrinfoResult));

    m_impl = info;
}

AddrInfo::~AddrInfo()
{
    freeaddrinfo(m_impl);
}

const std::pair<int, int>& AddrInfo::resolveSystemIdentifiers(Protocol protocol)
{
    switch(protocol)
    {
        case Protocol::Udp:
            return UDP_IDENTIFIERS;
        case Protocol::Tcp:
            return TCP_IDENTIFIERS;
    }

    return UDP_IDENTIFIERS;
}

}
