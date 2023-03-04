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

#include "iknet/UdpSocket.hpp"
#include "iknet/AddrInfo.hpp"
#include "iknet/systemspec.hpp"
#include "iknet/SocketInitializer.hpp"

#ifndef _WIN32
#include <netdb.h>
#include <arpa/inet.h>
#endif

namespace iknet
{

using ResolveAddressResult = ikgen::Result<AddrInfo, std::string>;
using CreateResult = ikgen::Result<UdpSocket, std::string>;
using IoResult = ikgen::Result<size_t, std::string>;
using LengthReceiveResult = ikgen::Result<UdpRecLength, std::string>;
using BufferReceiveResult = ikgen::Result<UdpRecBuffer, std::string>;


CreateResult UdpSocket::create(const std::string& listenAddress, uint16_t listenPort)
{
    try
    {
        return CreateResult::makeSuccess(listenAddress, listenPort);
    }
    catch(std::runtime_error& e)
    {
        return CreateResult::makeFailure(e.what());
    }
}

UdpSocket::UdpSocket(const std::string& listenAddress, uint16_t listenPort)
{
    // Initialize
    priv::SocketInitializer::initializeSockets();

    // Resolve listen address and port
    ResolveAddressResult resolveListenAddrResult = AddrInfo::resolve(listenAddress, listenPort, AddrInfo::Protocol::Udp);
    if(resolveListenAddrResult.isFailure())
        throw std::runtime_error(resolveListenAddrResult.getFailure());

    AddrInfo& listenAddr = resolveListenAddrResult.getSuccess();

    // Create and bind socket
    addrinfo* addr = listenAddr.getImpl();
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
        bindResult = bind(m_socketImpl, addr->ai_addr, static_cast<int>(addr->ai_addrlen));

        if(bindResult != 0)
        {
            error = lastNetworkErrorString();
            addr = addr->ai_next;
            iknet::closeSocket(m_socketImpl);
        }
    }

    if(isSocketInvalid(m_socketImpl))
        throw std::runtime_error("Failed to create UDP socket: " + error);

    if(bindResult != 0)
    {
        iknet::closeSocket(m_socketImpl);
        m_socketImpl = iknet::DEFAULT_INVALID_SOCKET;
        throw std::runtime_error("Failed to bind UDP socket: " + error);
    }

    m_addressFamily = addr->ai_family;
}

IoResult UdpSocket::send(const char* const buffer, size_t length, const std::string& remoteAddress, uint16_t remotePort)
{
    // Resolve address
    ResolveAddressResult resolveRemoteAddrResult = AddrInfo::resolve(remoteAddress, remotePort, AddrInfo::Protocol::Udp);
    if(resolveRemoteAddrResult.isFailure())
        return IoResult::makeFailure(resolveRemoteAddrResult.getFailure());

    AddrInfo& remoteAddr = resolveRemoteAddrResult.getSuccess();

    // Send
    int sendResult = iknet::DEFAULT_SOCKET_ERROR;
    addrinfo* addr = remoteAddr.getImpl();
    while(iknet::isSendError(sendResult) && addr != nullptr)
    {
        sendResult = ::sendto(m_socketImpl, buffer, static_cast<int>(length), 0, addr->ai_addr, static_cast<int>(addr->ai_addrlen));

        if(iknet::isSendError(sendResult))
            addr = addr->ai_next;
    }

    if(iknet::isSendError(sendResult))
        return IoResult::makeFailure("Failed to send data on UDP socket: " + lastNetworkErrorString());

    return IoResult::makeSuccess(sendResult);
}

ikgen::Result<ikgen::EmptyResult, std::string> UdpSocket::send(const Buffer& buffer, const std::string& remoteAddress, uint16_t remotePort)
{
    const ikgen::Result<size_t, std::string> sendResult = send(reinterpret_cast<const char*>(buffer.getData()), buffer.getSize(),
                                                        remoteAddress, remotePort);
    return sendResult.isSuccess()
            ? ikgen::Result<ikgen::EmptyResult, std::string>::makeSuccess()
            : ikgen::Result<ikgen::EmptyResult, std::string>::makeFailure(sendResult.getFailure());
}

LengthReceiveResult UdpSocket::receive(char* const buffer, size_t length)
{
    // Receive
    sockaddr_storage senderAddress;
    socklen_t addrSize = sizeof(sockaddr_storage);

    int receivedLength = -1;
#ifdef _WIN32
    receivedLength = ::recvfrom(m_socketImpl, buffer, static_cast<int>(length), 0, reinterpret_cast<sockaddr*>(&senderAddress), &addrSize);
#else
    receivedLength = ::recvfrom(m_socketImpl, buffer, length, 0, reinterpret_cast<sockaddr*>(&senderAddress), &addrSize);
#endif

    if(receivedLength < 0)
        return LengthReceiveResult::makeFailure("Failed to receive data from UDP socket: " + lastNetworkErrorString());

    if(senderAddress.ss_family == AF_INET)
    {
        sockaddr_in* senderAddressIn = reinterpret_cast<sockaddr_in*>(&senderAddress);
        char ipString[INET_ADDRSTRLEN];
        const char* const ntopResult = inet_ntop(AF_INET, &senderAddressIn->sin_addr, ipString, INET_ADDRSTRLEN);

        if(ntopResult == nullptr)
            return LengthReceiveResult::makeFailure("Failed to retreive sender information after receiving from UDP socket: " + lastNetworkErrorString());

        return LengthReceiveResult::makeSuccess(static_cast<size_t>(receivedLength), std::string(ipString), senderAddressIn->sin_port);
    }
    else if(senderAddress.ss_family == AF_INET6)
    {
        sockaddr_in6* senderAddressIn = reinterpret_cast<sockaddr_in6*>(&senderAddress);
        char ipString[INET6_ADDRSTRLEN];
        const char* const ntopResult = inet_ntop(AF_INET6, &senderAddressIn->sin6_addr, ipString, INET6_ADDRSTRLEN);

        if(ntopResult == nullptr)
            return LengthReceiveResult::makeFailure("Failed to retreive sender information after receiving from UDP socket: " + lastNetworkErrorString());

        return LengthReceiveResult::makeSuccess(static_cast<size_t>(receivedLength), std::string(ipString), senderAddressIn->sin6_port);
    }

    return LengthReceiveResult::makeFailure("Failed to retreive sender information after receiving from UDP socket");
}

BufferReceiveResult UdpSocket::receive()
{
    char buffer[UDP_MAX_LENGTH];

    const LengthReceiveResult receiveResult = receive(buffer, UDP_MAX_LENGTH);
    if(receiveResult.isFailure())
        return BufferReceiveResult::makeFailure(receiveResult.getFailure());

    const UdpRecLength& received = receiveResult.getSuccess();
    const size_t receivedLength = received.getLength();

    if(receivedLength < 0)
        return BufferReceiveResult::makeSuccess(Buffer(), std::move(received.getSender()));

    std::vector<std::byte> byteBuffer(receivedLength);
    std::memcpy(byteBuffer.data(), buffer, receivedLength);

    return BufferReceiveResult::makeSuccess(Buffer(byteBuffer), std::move(received.getSender()));
}

}
