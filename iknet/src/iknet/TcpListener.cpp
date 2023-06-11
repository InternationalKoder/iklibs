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

#include "iknet/TcpListener.hpp"
#include "iknet/AddrInfo.hpp"
#include "iknet/SocketInitializer.hpp"
#include "iknet/systemspec.hpp"

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif

namespace iknet
{

using ResolveAddressResult = ikgen::Result<AddrInfo, std::string>;
using CreateResult = ikgen::Result<TcpListener, std::string>;
using AcceptResult = ikgen::Result<TcpSocket, std::string>;

CreateResult TcpListener::create(const std::string& listenAddress, uint16_t listenPort, size_t maxWaitingConnections)
{
    try
    {
        return CreateResult::makeSuccess(listenAddress, listenPort, maxWaitingConnections);
    }
    catch(const std::runtime_error& e)
    {
        return CreateResult::makeFailure(e.what());
    }
}

TcpListener::TcpListener(const std::string& listenAddress, uint16_t listenPort, size_t maxWaitingConnections)
{
    // Initialize
    priv::SocketInitializer::initializeSockets();

    // Resolve listen address
    ResolveAddressResult listenAddrInfoRes = AddrInfo::resolve(listenAddress, listenPort, AddrInfo::Protocol::Tcp, true);
    if(listenAddrInfoRes.isFailure())
        throw std::runtime_error(listenAddrInfoRes.getFailure());

    AddrInfo& listenAddrInfo = listenAddrInfoRes.getSuccess();

    // Create and bind the socket
    addrinfo* addr = listenAddrInfo.getImpl();
    m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
    int bindResult = -1;
    std::string error = "";

    while(isSocketInvalid(m_socketImpl) && bindResult != 0 && addr != nullptr)
    {
        // Create
        m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
        m_socketImpl = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

        if(isSocketInvalid(m_socketImpl))
        {
            error = lastNetworkErrorString();
            addr = addr->ai_next;
            continue;
        }

#ifndef _WIN32
        // Enable reuse of address and port if in TIME_WAIT state
        const int enable = 1;
        setsockopt(m_socketImpl, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        setsockopt(m_socketImpl, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
#endif

        // Bind
        bindResult = bind(m_socketImpl, addr->ai_addr, static_cast<socklen_t>(addr->ai_addrlen));

        if(bindResult != 0)
        {
            error = lastNetworkErrorString();
            addr = addr->ai_next;
            iknet::closeSocket(m_socketImpl);
        }
    }

    if(isSocketInvalid(m_socketImpl))
        throw std::runtime_error("Failed to create TCP listener socket: " + error);

    if(bindResult != 0)
    {
        iknet::closeSocket(m_socketImpl);
        m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
        throw std::runtime_error("Failed to bind TCP listener socket: " + error);
    }

    // Listen to new connections
    if(::listen(m_socketImpl, static_cast<int>(maxWaitingConnections)) != 0)
    {
        iknet::closeSocket(m_socketImpl);
        m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
        throw std::runtime_error("Failed to listen on TCP listener socket: " + lastNetworkErrorString());
    }
}

TcpListener::~TcpListener()
{
    iknet::shutdownSocket(m_socketImpl);
    iknet::closeSocket(m_socketImpl);
}

AcceptResult TcpListener::accept()
{
    SocketImpl clientSocket = iknet::DEFAULT_INVALID_SOCKET;

    clientSocket = ::accept(m_socketImpl, NULL, NULL);
    if(isSocketInvalid(clientSocket))
        return AcceptResult::makeFailure("Failed to accept new connection on TCP listener socket: " + lastNetworkErrorString());

    return AcceptResult::makeSuccess(clientSocket);
}

}
