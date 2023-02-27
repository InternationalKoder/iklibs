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

#ifndef IKNET_TCP_SOCKET_HPP
#define IKNET_TCP_SOCKET_HPP

#include "Socket.hpp"
#include "Buffer.hpp"
#include "iknet_export.hpp"
#include <string>

namespace iknet
{

/*!
 * \brief A socket using TCP protocol
 */
class TcpSocket : public Socket
{
    public:

        /*!
         * \brief Creates a new instance of TcpSocket. Same as constructor but returns a Result
         * \param remoteAddress Remote address to connect to
         * \param remotePort Remote port to connect to
         * \return Either the newly created TcpSocket in case of success, or an error message otherwise
         */
        IKNET_EXPORT static Result<TcpSocket, std::string> create(const std::string& remoteAddress, uint16_t remotePort);


        /*!
         * \brief Constructor, throws std::runtime_exception if a problem occurs
         * \param remoteAddress Remote address to connect to
         * \param remotePort Remote port to connect to
         */
        IKNET_EXPORT TcpSocket(const std::string& remoteAddress, uint16_t remotePort);

        /*!
         * \brief Constructor to wrap an existing socket of the underlying system type
         * \param socketImpl The socket to wrap
         */
        IKNET_EXPORT explicit TcpSocket(SocketImpl socketImpl);


        /*!
         * \brief Sends the given buffer on the socket
         * \param buffer The buffer to send
         * \param length The length of the buffer in bytes
         * \return Either the size of the effictively sent buffer in case of success, an error message otherwise
         */
        IKNET_EXPORT Result<size_t, std::string> send(const char* const buffer, size_t length);

        /*!
         * \brief Send the given buffer on the socket
         * \param buffer The buffer to send
         * \return Nothing in case of success, an error message otherwise
         */
        IKNET_EXPORT Result<EmptyResult, std::string> send(const Buffer& buffer);


        /*!
         * \brief Receives data from the socket and stores it in a buffer
         * \param buffer [out] The buffer that will be filled with received data
         * \param length The number of bytes to receive, the given buffer must be at least this length
         * \return Either the size of the effictively received data in case of success, an error message otherwise
         */
        IKNET_EXPORT Result<size_t, std::string> receive(char* const buffer, size_t length);

        /*!
         * \brief Receives data from the socket and stores it in a buffer
         * \return A buffer filled with the received data in case of success, an error message otherwise
         */
        IKNET_EXPORT Result<Buffer, std::string> receive();


    private:

        static constexpr size_t TCP_MAX_LENGTH = 65535;
};

}

#endif // IKNET_TCP_SOCKET_HPP
