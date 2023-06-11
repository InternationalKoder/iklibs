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

#ifndef IKNET_TCP_LISTENER_HPP
#define IKNET_TCP_LISTENER_HPP

#include <string>
#include "TcpSocket.hpp"
#include "iknet_export.hpp"
#include <ikgen/Result.hpp>

namespace iknet
{

/*!
 * \brief Listener that allows to receive incoming TCP connections
 */
class TcpListener
{
    public:

        /*!
         * \brief Creates a new instance of TcpListener. Same as constructor but returns a Result
         * \param listenAddress Address to listen to for incoming connections
         * \param listenPort Port to listen to for incoming connections
         * \param maxWaitingConnections Maximum allowed count of pending connections in queue waiting to be accepted
         * \return Either the newly created TcpListener in case of success, or an error message otherwise
         */
        IKNET_EXPORT static ikgen::Result<TcpListener, std::string> create(const std::string& listenAddress, uint16_t listenPort,
                                                                           size_t maxWaitingConnections = DEFAULT_MAX_WAITING_CONNECTIONS);


        /*!
         * \brief Constructor, throws std::runtime_error if a problem occurs
         * \param listenAddress Address to listen to for incoming connections
         * \param listenPort Port to listen to for incoming connections
         * \param maxWaitingConnections Maximum allowed count of pending connections in queue waiting to be accepted
         */
        IKNET_EXPORT TcpListener(const std::string& listenAddress, uint16_t listenPort,
                                 size_t maxWaitingConnections = DEFAULT_MAX_WAITING_CONNECTIONS);

        TcpListener(const TcpListener& other) = delete;
        TcpListener(TcpListener&& other) = delete;
        IKNET_EXPORT ~TcpListener();


        /*!
         * \brief Accepts one pending connection and opens a TCP socket
         * \return Either a new TcpSocket for the newly accepted connection in case of success, or an error message
         */
        IKNET_EXPORT ikgen::Result<TcpSocket, std::string> accept();

    private:

#ifdef _WIN32
        static constexpr size_t DEFAULT_MAX_WAITING_CONNECTIONS = SOMAXCONN;
#else
        static constexpr size_t DEFAULT_MAX_WAITING_CONNECTIONS = 128;
#endif

        SocketImpl m_socketImpl;
};

}

#endif // IKNET_TCP_LISTENER_HPP
