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

#ifndef IKNET_UDP_SOCKET_HPP
#define IKNET_UDP_SOCKET_HPP

#include "Socket.hpp"
#include "Buffer.hpp"
#include "UdpRecBuffer.hpp"
#include "UdpRecLength.hpp"
#include "iknet_export.hpp"
#include <string>

#if defined(_WIN32) && !defined(IKNET_OVERRIDE_SA_FAMILY_T)
using sa_family_t = unsigned short;
#endif

#ifndef _WIN32
#include <sys/socket.h>
#endif

struct addrinfo;

namespace iknet
{

/*!
 * \brief A socket using UDP protocol
 */
class UdpSocket : public Socket
{
    public:

        /*!
         * \brief Creates a new instance of UdpSocket. Same as constructor but returns a Result
         * \param listenAddress Address to listen to for incoming packets
         * \param listenPort Port to bind on local device
         * \return Either the newly created UdpSocket in case of success, or an error message otherwise
         */
        IKNET_EXPORT static ikgen::Result<UdpSocket, std::string> create(const std::string& listenAddress, uint16_t listenPort);


        /*!
         * \brief Constructor, throws std::runtime_exception if a problem occurs
         * \param listenAddress Address to listen to for incoming packets
         * \param listenPort Port to bind on local device
         */
        IKNET_EXPORT UdpSocket(const std::string& listenAddress, uint16_t listenPort);


        /*!
         * \brief Sends the given buffer on the socket
         * \param buffer The buffer to send
         * \param length The length of the buffer in bytes
         * \param remoteAddress Remote address the buffer will be sent to
         * \param remotePort Remote port the buffer will be sent to
         * \return Either the size of the effictively sent buffer in case of success, an error message otherwise
         */
        IKNET_EXPORT ikgen::Result<size_t, std::string> send(const char* const buffer, size_t length,
                                                             const std::string& remoteAddress, uint16_t remotePort);

        /*!
         * \brief Send the given buffer on the socket
         * \param buffer The buffer to send
         * \param remoteAddress Remote address the buffer will be sent to
         * \param remotePort Remote port the buffer will be sent to
         * \return Nothing in case of success, an error message otherwise
         */
        IKNET_EXPORT ikgen::Result<ikgen::EmptyResult, std::string> send(const Buffer& buffer, const std::string& remoteAddress, uint16_t remotePort);


        /*!
         * \brief Receives data from the socket and stores it in a buffer
         * \param buffer [out] The buffer that will be filled with received data
         * \param length The number of bytes to receive, the given buffer must be at least this length
         * \return Either the size of the effictively received data with the sender host and port in case of success, an error message otherwise
         */
        IKNET_EXPORT ikgen::Result<UdpRecLength, std::string> receive(char* const buffer, size_t length);

        /*!
         * \brief Receives data from the socket and stores it in a buffer
         * \return A buffer filled with the received data and the sender's host and port in case of success, an error message otherwise
         */
        IKNET_EXPORT ikgen::Result<UdpRecBuffer, std::string> receive();


    private:

        static constexpr size_t UDP_MAX_LENGTH = 65507;

        sa_family_t m_addressFamily;
};

}

#endif // IKNET_UDP_SOCKET_HPP
