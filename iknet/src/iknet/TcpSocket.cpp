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

#include "iknet/TcpSocket.hpp"
#include "iknet/AddrInfo.hpp"
#include "iknet/systemspec.hpp"
#include "iknet/SocketInitializer.hpp"

#ifndef _WIN32
#include <netdb.h>
#endif

namespace iknet
{

using ResolveAddressResult = ikgen::Result<AddrInfo, std::string>;
using CreateResult = ikgen::Result<TcpSocket, std::string>;
using IoResult = ikgen::Result<size_t, std::string>;
using ReceiveResult = ikgen::Result<Buffer, std::string>;

#ifdef _WIN32
using IoSize = int;
#else
using IoSize = ssize_t;
#endif


CreateResult TcpSocket::create(const std::string& remoteAddress, uint16_t remotePort)
{
    try
    {
        return CreateResult::makeSuccess(remoteAddress, remotePort);
    }
    catch(const std::runtime_error& e)
    {
        return CreateResult::makeFailure(e.what());
    }
}

TcpSocket::TcpSocket(const std::string& remoteAddress, uint16_t remotePort)
{
    // Initialize
    priv::SocketInitializer::initializeSockets();

    // Resolve remote address and port
    ResolveAddressResult remoteAddrInfoRes = AddrInfo::resolve(remoteAddress, remotePort, AddrInfo::Protocol::Tcp);
    if(remoteAddrInfoRes.isFailure())
        throw std::runtime_error(remoteAddrInfoRes.getFailure());

    AddrInfo& remoteAddrInfo = remoteAddrInfoRes.getSuccess();

    // Create and connect the socket
    addrinfo* addr = remoteAddrInfo.getImpl();
    m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
    int connectResult = -1;
    std::string error = "";

    while(isSocketInvalid(m_socketImpl) && connectResult != 0 && addr != nullptr)
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

        // Connect
        connectResult = connect(m_socketImpl, addr->ai_addr, static_cast<socklen_t>(addr->ai_addrlen));

        if(connectResult != 0)
        {
            error = lastNetworkErrorString();
            addr = addr->ai_next;
            iknet::closeSocket(m_socketImpl);
        }
    }

    if(isSocketInvalid(m_socketImpl))
        throw std::runtime_error("Failed to create TCP socket: " + error);

    if(connectResult != 0)
    {
        iknet::closeSocket(m_socketImpl);
        m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
        throw std::runtime_error("Failed to connect TCP socket: " + error);
    }
}

TcpSocket::TcpSocket(SocketImpl socketImpl) :
    Socket(std::move(socketImpl))
{}

IoResult TcpSocket::send(const char* const buffer, size_t length)
{
#ifdef _WIN32
    const IoSize sendResult = ::send(m_socketImpl, buffer, static_cast<int>(length), 0);
#else
    const IoSize sendResult = ::send(m_socketImpl, buffer, length, 0);
#endif

    if(isSocketInvalid(sendResult))
        return IoResult::makeFailure("Failed to send data on TCP socket: " + lastNetworkErrorString());

    return IoResult::makeSuccess(sendResult);
}

ikgen::Result<ikgen::EmptyResult, std::string> TcpSocket::send(const Buffer& buffer)
{
    const ikgen::Result<size_t, std::string> sendResult = send(reinterpret_cast<const char*>(buffer.getData()), buffer.getSize());
    return sendResult.isSuccess()
            ? ikgen::Result<ikgen::EmptyResult, std::string>::makeSuccess()
            : ikgen::Result<ikgen::EmptyResult, std::string>::makeFailure(sendResult.getFailure());
}

IoResult TcpSocket::receive(char* const buffer, size_t length)
{
#ifdef _WIN32
    const IoSize receivedLength = ::recv(m_socketImpl, buffer, static_cast<int>(length), 0);
#else
    const IoSize receivedLength = ::recv(m_socketImpl, buffer, length, 0);
#endif

    if(receivedLength < 0)
        return IoResult::makeFailure("Failed to receive data from TCP socket: " + lastNetworkErrorString());

    return IoResult::makeSuccess(static_cast<size_t>(receivedLength));
}

ReceiveResult TcpSocket::receive()
{
    char buffer[TCP_MAX_LENGTH];

    const ikgen::Result<size_t, std::string> receiveResult = receive(buffer, TCP_MAX_LENGTH);
    if(receiveResult.isFailure())
        return ReceiveResult::makeFailure(receiveResult.getFailure());

    const size_t receivedLength = receiveResult.getSuccess();

    if(receivedLength < 0)
        return ReceiveResult::makeSuccess();

    std::vector<std::byte> byteBuffer(receivedLength);
    std::memcpy(byteBuffer.data(), buffer, receivedLength);

    return ReceiveResult::makeSuccess(byteBuffer);
}

}
