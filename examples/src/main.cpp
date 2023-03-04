/*
    Copyright (C) 2019, 2020, 2023, InternationalKoder

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

#include "Settings.hpp"
#include <iostream>
#include <iklog/Log.hpp>
#include <iklog/outputs/RollingFileOutput.hpp>
#include <ikconf/readers/PropertiesReader.hpp>
#include <ikconf/readers/JsonReader.hpp>
#include <ikconf/exceptions/ConfigurationException.hpp>
#include <ikparll/ThreadPool.hpp>
#include <iklogconf/LogConfigurator.hpp>
#include <iknet/UdpSocket.hpp>
#include <iknet/TcpListener.hpp>
#include <iknet/TcpSocket.hpp>
#include <iknet/SocketPoller.hpp>

using namespace iklog::literals;
using namespace std::chrono_literals;

int main()
{
    // -------- IKLOG
    // create a logger with a name and enabled logging levels, then try to log some messages in different levels
    iklog::Log log("iklibs-examples", iklog::Level::DEBUG | iklog::Level::WARNING | iklog::Level::ERROR);
    log.info("Won't be displayed because the level is not activated");
    log.warn("This warning should be displayed");

    // retrieve a logger from its name
    iklog::Log* examplesLog = iklog::Log::getLog("iklibs-examples");
    examplesLog->disableLevels(iklog::Level::DEBUG);
    log.debug("We retrieved a Log object and disabled debug level on it, this message should not appear");

    iklog::RollingFileOutput rollingFileOutput("examples.log", 512_b, 5);
    iklog::Log rollingFileLog("rolling-file", iklog::Level::INFO, rollingFileOutput);

    for(unsigned int i = 0 ; i < 80 ; i++)
        rollingFileLog.info("Line number " + std::to_string(i));

    // -------- IKCONF
    // create the object that will hold our configuration
    Settings settings;

    // read from a .properties file
    ikconf::PropertiesReader propertiesReader(settings);

    try
    {
        propertiesReader.read("resources/test.properties");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    // display the read values from the file
    std::cout << "Properties file:" << std::endl;
    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestCharAsNumber() << std::endl;


    // now read from a .json file
    ikconf::JsonReader jsonReader(settings);

    try
    {
        jsonReader.read("resources/test.json");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting configuration: " << e.what() << std::endl;
    }

    // display the read values from the file
    std::cout << "JSON file:" << std::endl;
    std::cout << settings.getTestString() << std::endl;
    std::cout << settings.getTestInt() << std::endl;
    std::cout << settings.getTestFloat() << std::endl;
    std::cout << settings.getTestBool() << std::endl;
    std::cout << settings.getTestCharAsChar() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubString() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubInt() << std::endl;
    std::cout << settings.getTestSubSettings().getTestSubSubSettings().getTestSubSubFloat() << std::endl;
    std::cout << settings.getTestOtherSubSettings().getTestOtherSubSettingsStr() << std::endl;

    for(const int& i : settings.getTestArray())
    {
        std::cout << "array item: " << i << std::endl;
    }

    for(const std::string& i : settings.getTestStringArray())
    {
        std::cout << "string array item: " << i << std::endl;
    }

    for(const SubSettingsArrayItem& i : settings.getTestSubSettingsArray())
    {
        std::cout << "object array item: { " << i.getIndex() << " ; " << i.getValue() << " }" << std::endl;
    }

    // -------- IKPARLL
    std::cout << "Thread pool test" << std::endl;
    ikparll::ThreadPool<int, 3> threadPool([](int i) { std::cout << std::this_thread::get_id() << " : " << i*2 << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });
    for(int i = 0 ; i < 10 ; i++)
    {
        threadPool.addItem(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    // -------- IKLOGCONF
    iklogconf::LogConfigurator logConfigurator;
    try
    {
        logConfigurator.readJsonFile("resources/log.json");
    }
    catch(const ikconf::ConfigurationException& e)
    {
        std::cerr << "Error while getting log configuration: " << e.what() << std::endl;
    }

    iklog::Log* const iklogconfStdoutLogger = iklog::Log::getLog("iklogconfStdoutLogger");
    iklogconfStdoutLogger->info("This log has been configured from a file!");
    iklogconfStdoutLogger->debug("This message should not be displayed");

    iklog::Log* const iklogconfRollingFileLogger = iklog::Log::getLog("iklogconfRollingFileLogger");
    for(unsigned int i = 0 ; i < 10 ; i++)
    {
        iklogconfRollingFileLogger->info("Message " + std::to_string(i) + " on log configured from file");
    }

    iklog::Log* const iklogconfSingleOutput1 = iklog::Log::getLog("iklogconfSingleOutput-1");
    iklog::Log* const iklogconfSingleOutput2 = iklog::Log::getLog("iklogconfSingleOutput-2");

    iklogconfSingleOutput1->error("this log uses");
    iklogconfSingleOutput2->warn("the same output as this one");
    iklogconfSingleOutput1->error("and it works very well");

    // -------- IKNET

    iknet::Result<iknet::Result<int, std::string>, int> res = iknet::Result<iknet::Result<int, std::string>, int>::makeSuccess(iknet::Result<int, std::string>::makeSuccess(19));
    std::cout << res.getSuccess().getSuccess() << std::endl;

    {
        auto udpSocket1CreateResult = iknet::UdpSocket::create("127.0.0.1", 46721);
        if(udpSocket1CreateResult.isFailure())
        {
            std::cerr << udpSocket1CreateResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::UdpSocket& udpSocket1 = udpSocket1CreateResult.getSuccess();

        auto udpSocket2CreateResult = iknet::UdpSocket::create("127.0.0.1", 46722);
        if(udpSocket2CreateResult.isFailure())
        {
            std::cerr << udpSocket2CreateResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::UdpSocket& udpSocket2 = udpSocket2CreateResult.getSuccess();

        {
            const char* const sendBuffer = "hello 123";
            std::cout << "Sending buffer \"" << sendBuffer << "\" on UDP socket" << std::endl;
            const auto sendResult = udpSocket1.send(sendBuffer, 9, "127.0.0.1", 46722);
            if(sendResult.isFailure())
            {
                std::cerr << sendResult.getFailure() << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << "Successfully sent " << sendResult.getSuccess() << " bytes" << std::endl;
        }

        {
            char receiveBuffer[10];
            receiveBuffer[9] = '\0';
            const auto receiveResult = udpSocket2.receive(receiveBuffer, 9);
            if(receiveResult.isFailure())
            {
                std::cerr << receiveResult.getFailure() << std::endl;
                return EXIT_FAILURE;
            }
            const iknet::UdpRecLength& received = receiveResult.getSuccess();
            std::cout << "Received buffer \"" << receiveBuffer << "\" on UDP socket with size " << received.getLength() <<
                         " from " << received.getSender().getHost() << ':' << received.getSender().getPort() << std::endl;
        }

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
                return EXIT_FAILURE;
            }
            std::cout << "Successfully sent buffer" << std::endl;
        }

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
                return EXIT_FAILURE;
            }

            iknet::UdpRecBuffer& received = receiveResult.getSuccess();
            iknet::Buffer& receivedBuffer = received.getBuffer();

            receivedBuffer >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
            std::cout << "Received values " << v1 << " (uint32_t), " << v2 << " (uint64_t), \"" << v3 << "\" (std::string), " <<
                         v4 << " (char), " << v5 << " (double), " << v6 << " (int8_t) on UDP socket from " <<
                         received.getSender().getHost() << ':' << received.getSender().getPort() << std::endl;
        }
    }

    {
        auto tcpListenerResult = iknet::TcpListener::create("::1", 37684);
        if(tcpListenerResult.isFailure())
        {
            std::cerr << tcpListenerResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpListener& tcpListener = tcpListenerResult.getSuccess();

        auto tcpSocket1Result = iknet::TcpSocket::create("::1", 37684);
        if(tcpSocket1Result.isFailure())
        {
            std::cerr << tcpSocket1Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocket1 = tcpSocket1Result.getSuccess();

        auto tcpSocket2Result = tcpListener.accept();
        if(tcpSocket2Result.isFailure())
        {
            std::cerr << tcpSocket2Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocket2 = tcpSocket2Result.getSuccess();

        {
            const char* const sendBuffer = "321 hello";
            std::cout << "Sending buffer \"" << sendBuffer << "\" on TCP socket" << std::endl;
            const auto sendResult = tcpSocket1.send(sendBuffer, 9);
            if(sendResult.isFailure())
            {
                std::cerr << sendResult.getFailure() << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << "Successfully sent " << sendResult.getSuccess() << " bytes" << std::endl;
        }

        {
            char receiveBuffer[10];
            receiveBuffer[9] = '\0';

            const auto receiveResult = tcpSocket2.receive(receiveBuffer, 9);
            if(receiveResult.isFailure())
            {
                std::cerr << receiveResult.getFailure() << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << "Received buffer \"" << receiveBuffer << "\" on TCP socket with size " << receiveResult.getSuccess() << std::endl;
        }

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
                return EXIT_FAILURE;
            }
            std::cout << "Successfully sent buffer" << std::endl;
        }

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
                return EXIT_FAILURE;
            }
            iknet::Buffer buffer = receiveResult.getSuccess();

            buffer >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
            std::cout << "Received values " << v1 << " (double), " << v2 << " (std::string), \"" << v3 << "\" (int8_t), "
                      << v4 << " (uint64_t), " << v5 << " (char), " << v6 << " (uint32_t) on TCP socket" << std::endl;
        }
    }

    {
        auto tcpListenerResult = iknet::TcpListener::create("localhost", 37684);
        if(tcpListenerResult.isFailure())
        {
            std::cerr << tcpListenerResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpListener& tcpListener = tcpListenerResult.getSuccess();

        auto tcpSocketOut1Result = iknet::TcpSocket::create("localhost", 37684);
        if(tcpSocketOut1Result.isFailure())
        {
            std::cerr << tcpSocketOut1Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }

        auto tcpSocketOut2Result = iknet::TcpSocket::create("localhost", 37684);
        if(tcpSocketOut2Result.isFailure())
        {
            std::cerr << tcpSocketOut2Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocketOut2 = tcpSocketOut2Result.getSuccess();

        auto tcpSocketOut3Result = iknet::TcpSocket::create("localhost", 37684);
        if(tcpSocketOut3Result.isFailure())
        {
            std::cerr << tcpSocketOut3Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }

        auto tcpSocketIn1Result = tcpListener.accept();
        if(tcpSocketIn1Result.isFailure())
        {
            std::cerr << tcpSocketIn1Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocketIn1 = tcpSocketIn1Result.getSuccess();

        auto tcpSocketIn2Result = tcpListener.accept();
        if(tcpSocketIn2Result.isFailure())
        {
            std::cerr << tcpSocketIn2Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocketIn2 = tcpSocketIn2Result.getSuccess();

        auto tcpSocketIn3Result = tcpListener.accept();
        if(tcpSocketIn3Result.isFailure())
        {
            std::cerr << tcpSocketIn3Result.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::TcpSocket& tcpSocketIn3 = tcpSocketIn3Result.getSuccess();

        iknet::SocketPoller<iknet::TcpSocket> socketPoller;
        socketPoller.add(tcpSocketIn1);
        socketPoller.add(tcpSocketIn2);
        socketPoller.add(tcpSocketIn3);

        const auto pollResult1 = socketPoller.poll(100ms);
        if(pollResult1.isFailure())
        {
            std::cerr << pollResult1.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        const std::optional<const iknet::TcpSocket*> poll1 = pollResult1.getSuccess();
        if(poll1.has_value())
        {
            std::cerr << "SocketPoller got an unexpected result" << std::endl;
            return EXIT_FAILURE;
        }

        iknet::Buffer buffer1;
        buffer1 << "test poll";

        const auto sendResult = tcpSocketOut2.send(buffer1);
        if(sendResult.isFailure())
        {
            std::cerr << sendResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Successfully sent buffer" << std::endl;

        const auto pollResult2 = socketPoller.poll(100ms);
        if(pollResult2.isFailure())
        {
            std::cerr << pollResult2.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        const std::optional<iknet::TcpSocket*> poll2 = pollResult2.getSuccess();
        if(!poll2.has_value())
        {
            std::cerr << "SocketPoller got an unexpected result" << std::endl;
            return EXIT_FAILURE;
        }

        if(poll2.value() != &tcpSocketIn2)
        {
            std::cerr << "SocketPoller returned an unexpected socket" << std::endl;
            return EXIT_FAILURE;
        }

        const auto receiveResult = poll2.value()->receive();
        if(receiveResult.isFailure())
        {
            std::cerr << receiveResult.getFailure() << std::endl;
            return EXIT_FAILURE;
        }
        iknet::Buffer receivedBuffer = receiveResult.getSuccess();

        std::string receivedString;
        receivedBuffer >> receivedString;
        std::cout << "Received with poller: " << receivedString << std::endl;
    }

    return EXIT_SUCCESS;
}
