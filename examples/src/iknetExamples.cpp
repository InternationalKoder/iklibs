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

#include "iknetExamples.hpp"

#include <iknet/UdpSocket.hpp>
#include <iknet/TcpListener.hpp>
#include <iknet/TcpSocket.hpp>
#include <iknet/SocketPoller.hpp>
#include <iostream>

using namespace std::chrono_literals;

void runIknetExamples()
{
    runUdpSocketsExamples();
    runTcpSocketsExamples();
    runSocketPollerExamples();
}

void runUdpSocketsExamples()
{
    // Create UDP socket 1
    auto udpSocket1CreateResult = iknet::UdpSocket::create("127.0.0.1", 46721);
    if(udpSocket1CreateResult.isFailure())
    {
        std::cerr << udpSocket1CreateResult.getFailure() << std::endl;
        return;
    }
    iknet::UdpSocket& udpSocket1 = udpSocket1CreateResult.getSuccess();

    // Create UDP socket 2
    auto udpSocket2CreateResult = iknet::UdpSocket::create("127.0.0.1", 46722);
    if(udpSocket2CreateResult.isFailure())
    {
        std::cerr << udpSocket2CreateResult.getFailure() << std::endl;
        return;
    }
    iknet::UdpSocket& udpSocket2 = udpSocket2CreateResult.getSuccess();

    // Send C-style buffer from UDP socket 1 to UDP socket 2
    {
        const char* const sendBuffer = "hello 123";
        std::cout << "Sending buffer \"" << sendBuffer << "\" on UDP socket" << std::endl;
        const auto sendResult = udpSocket1.send(sendBuffer, 9, "127.0.0.1", 46722);
        if(sendResult.isFailure())
        {
            std::cerr << sendResult.getFailure() << std::endl;
            return;
        }
        std::cout << "Successfully sent " << sendResult.getSuccess() << " bytes" << std::endl;
    }

    // Receive C-style buffer on UDP socket 2
    {
        char receiveBuffer[10];
        receiveBuffer[9] = '\0';
        const auto receiveResult = udpSocket2.receive(receiveBuffer, 9);
        if(receiveResult.isFailure())
        {
            std::cerr << receiveResult.getFailure() << std::endl;
            return;
        }
        const iknet::UdpRecLength& received = receiveResult.getSuccess();
        std::cout << "Received buffer \"" << receiveBuffer << "\" on UDP socket with size " << received.getLength() <<
                     " from " << received.getSender().getHost() << ':' << received.getSender().getPort() << std::endl;
    }

    // Send iknet buffer from UDP socket 2 to UDP socket 1
    {
        iknet::Buffer buffer;
        const uint32_t v1 = 42;
        const uint64_t v2 = 72;
        const std::string v3 = "hello, world!";
        const char v4 = 'c';
        const double v5 = 8465.247;
        const int8_t v6 = -19;
        buffer << v1 << v2 << v3 << v4 << v5 << v6;
        std::cout << "Sending values " << v1 << " (uint32_t), " << v2 << " (uint64_t), \"" << v3 << "\" (std::string), "
                  << v4 << " (char), " << v5 << " (double), " << v6 << " (int8_t) on UDP socket" << std::endl;

        const auto sendResult = udpSocket2.send(buffer, "127.0.0.1", 46721);
        if(sendResult.isFailure())
        {
            std::cerr << sendResult.getFailure() << std::endl;
            return;
        }
        std::cout << "Successfully sent buffer" << std::endl;
    }

    // Receive iknet buffer on UDP socket 1
    {
        uint32_t v1;
        uint64_t v2;
        std::string v3;
        char v4;
        double v5;
        int8_t v6;

        auto receiveResult = udpSocket1.receive();
        if(receiveResult.isFailure())
        {
            std::cerr << receiveResult.getFailure() << std::endl;
            return;
        }

        iknet::UdpRecBuffer& received = receiveResult.getSuccess();
        iknet::Buffer& receivedBuffer = received.getBuffer();

        receivedBuffer >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
        std::cout << "Received values " << v1 << " (uint32_t), " << v2 << " (uint64_t), \"" << v3 << "\" (std::string), " <<
                     v4 << " (char), " << v5 << " (double), " << v6 << " (int8_t) on UDP socket from " <<
                     received.getSender().getHost() << ':' << received.getSender().getPort() << std::endl;
    }
}

void runTcpSocketsExamples()
{
    // Create TCP listener
    auto tcpListenerResult = iknet::TcpListener::create("::1", 37684);
    if(tcpListenerResult.isFailure())
    {
        std::cerr << tcpListenerResult.getFailure() << std::endl;
        return;
    }
    iknet::TcpListener& tcpListener = tcpListenerResult.getSuccess();

    // Create TCP socket 1 and connect to TCP listener
    auto tcpSocket1Result = iknet::TcpSocket::create("::1", 37684);
    if(tcpSocket1Result.isFailure())
    {
        std::cerr << tcpSocket1Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocket1 = tcpSocket1Result.getSuccess();

    // Create TCP socket 2 by accepting incoming connection on TCP listener
    auto tcpSocket2Result = tcpListener.accept();
    if(tcpSocket2Result.isFailure())
    {
        std::cerr << tcpSocket2Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocket2 = tcpSocket2Result.getSuccess();

    // Send C-style buffer from TCP socket 1 to TCP socket 2
    {
        const char* const sendBuffer = "321 hello";
        std::cout << "Sending buffer \"" << sendBuffer << "\" on TCP socket" << std::endl;
        const auto sendResult = tcpSocket1.send(sendBuffer, 9);
        if(sendResult.isFailure())
        {
            std::cerr << sendResult.getFailure() << std::endl;
            return;
        }
        std::cout << "Successfully sent " << sendResult.getSuccess() << " bytes" << std::endl;
    }

    // Receive C-style buffer on TCP socket 2
    {
        char receiveBuffer[10];
        receiveBuffer[9] = '\0';

        const auto receiveResult = tcpSocket2.receive(receiveBuffer, 9);
        if(receiveResult.isFailure())
        {
            std::cerr << receiveResult.getFailure() << std::endl;
            return;
        }
        std::cout << "Received buffer \"" << receiveBuffer << "\" on TCP socket with size " << receiveResult.getSuccess() << std::endl;
    }

    // Send iknet buffer from TCP socket 2 to TCP socket 1
    {
        iknet::Buffer buffer;
        const double v1 = 749840.45648;
        const std::string v2 = "example";
        const int8_t v3 = -123;
        const uint64_t v4 = 756891;
        const char v5 = 'p';
        const uint32_t v6 = 4769;
        buffer << v1 << v2 << v3 << v4 << v5 << v6;
        std::cout << "Sending values " << v1 << " (double), " << v2 << " (std::string), \"" << v3 << "\" (int8_t), "
                  << v4 << " (uint64_t), " << v5 << " (char), " << v6 << " (uint32_t) on TCP socket" << std::endl;

        const auto sendResult = tcpSocket2.send(buffer);
        if(sendResult.isFailure())
        {
            std::cerr << sendResult.getFailure() << std::endl;
            return;
        }
        std::cout << "Successfully sent buffer" << std::endl;
    }

    // Receive iknet buffer on TCP socket 1
    {
        double v1;
        std::string v2;
        int8_t v3;
        uint64_t v4;
        char v5;
        uint32_t v6;

        const auto receiveResult = tcpSocket1.receive();
        if(receiveResult.isFailure())
        {
            std::cerr << receiveResult.getFailure() << std::endl;
            return;
        }
        iknet::Buffer buffer = receiveResult.getSuccess();

        buffer >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
        std::cout << "Received values " << v1 << " (double), " << v2 << " (std::string), \"" << v3 << "\" (int8_t), "
                  << v4 << " (uint64_t), " << v5 << " (char), " << v6 << " (uint32_t) on TCP socket" << std::endl;
    }
}

void runSocketPollerExamples()
{
    // Create TCP listener
    auto tcpListenerResult = iknet::TcpListener::create("localhost", 37684);
    if(tcpListenerResult.isFailure())
    {
        std::cerr << tcpListenerResult.getFailure() << std::endl;
        return;
    }
    iknet::TcpListener& tcpListener = tcpListenerResult.getSuccess();

    // Create TCP outbound socket 1 and connect to TCP listener
    auto tcpSocketOut1Result = iknet::TcpSocket::create("localhost", 37684);
    if(tcpSocketOut1Result.isFailure())
    {
        std::cerr << tcpSocketOut1Result.getFailure() << std::endl;
        return;
    }

    // Create TCP outbound socket 2 and connect to TCP listener
    auto tcpSocketOut2Result = iknet::TcpSocket::create("localhost", 37684);
    if(tcpSocketOut2Result.isFailure())
    {
        std::cerr << tcpSocketOut2Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocketOut2 = tcpSocketOut2Result.getSuccess();

    // Create TCP outbound socket 3 and connect to TCP listener
    auto tcpSocketOut3Result = iknet::TcpSocket::create("localhost", 37684);
    if(tcpSocketOut3Result.isFailure())
    {
        std::cerr << tcpSocketOut3Result.getFailure() << std::endl;
        return;
    }

    // Create TCP inbound socket 1 by accepting incoming TCP connection on TCP listener
    auto tcpSocketIn1Result = tcpListener.accept();
    if(tcpSocketIn1Result.isFailure())
    {
        std::cerr << tcpSocketIn1Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocketIn1 = tcpSocketIn1Result.getSuccess();

    // Create TCP inbound socket 2 by accepting incoming TCP connection on TCP listener
    auto tcpSocketIn2Result = tcpListener.accept();
    if(tcpSocketIn2Result.isFailure())
    {
        std::cerr << tcpSocketIn2Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocketIn2 = tcpSocketIn2Result.getSuccess();

    // Create TCP inbound socket 3 by accepting incoming TCP connection on TCP listener
    auto tcpSocketIn3Result = tcpListener.accept();
    if(tcpSocketIn3Result.isFailure())
    {
        std::cerr << tcpSocketIn3Result.getFailure() << std::endl;
        return;
    }
    iknet::TcpSocket& tcpSocketIn3 = tcpSocketIn3Result.getSuccess();

    // Create SocketPoller and add all TCP inbound sockets
    iknet::SocketPoller<iknet::TcpSocket> socketPoller;
    socketPoller.add(tcpSocketIn1);
    socketPoller.add(tcpSocketIn2);
    socketPoller.add(tcpSocketIn3);

    // Poll with no incoming data: should hit timeout without error
    const auto pollResult1 = socketPoller.poll(100ms);
    if(pollResult1.isFailure())
    {
        std::cerr << pollResult1.getFailure() << std::endl;
        return;
    }
    const std::optional<const iknet::TcpSocket*> poll1 = pollResult1.getSuccess();
    if(poll1.has_value())
    {
        std::cerr << "SocketPoller got an unexpected result" << std::endl;
        return;
    }

    // Send data on TCP outbound socket 2
    iknet::Buffer buffer1;
    buffer1 << "test poll";

    const auto sendResult = tcpSocketOut2.send(buffer1);
    if(sendResult.isFailure())
    {
        std::cerr << sendResult.getFailure() << std::endl;
        return;
    }
    std::cout << "Successfully sent buffer" << std::endl;

    // Receive data TCP inbound sockets by polling
    const auto pollResult2 = socketPoller.poll(100ms);
    if(pollResult2.isFailure())
    {
        std::cerr << pollResult2.getFailure() << std::endl;
        return;
    }
    const std::optional<iknet::TcpSocket*> poll2 = pollResult2.getSuccess();
    if(!poll2.has_value())
    {
        std::cerr << "SocketPoller got an unexpected result" << std::endl;
        return;
    }

    // Incoming data should arrive on inbound socket 2
    if(poll2.value() != &tcpSocketIn2)
    {
        std::cerr << "SocketPoller returned an unexpected socket" << std::endl;
        return;
    }

    const auto receiveResult = poll2.value()->receive();
    if(receiveResult.isFailure())
    {
        std::cerr << receiveResult.getFailure() << std::endl;
        return;
    }
    iknet::Buffer receivedBuffer = receiveResult.getSuccess();

    std::string receivedString;
    receivedBuffer >> receivedString;
    std::cout << "Received with poller: " << receivedString << std::endl;
}
