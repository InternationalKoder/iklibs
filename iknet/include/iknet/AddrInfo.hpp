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

#ifndef IKNET_ADDR_INFO_HPP
#define IKNET_ADDR_INFO_HPP

#include "iknet_export.hpp"
#include <ikgen/Result.hpp>
#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

struct addrinfo;

namespace iknet
{

/*!
 * \brief Wrapper around the POSIX addrinfo structure
 */
class AddrInfo
{
    public:

        enum class Protocol { Udp, Tcp };

        /*!
         * \brief Resolves a host identifier. The obtained information can be used with sockets to identify the host
         * \param host Host name to resolve (can be IP address or domain name for example)
         * \param port Port to reach
         * \param protocol Protocol that will be used to communicate with the host
         * \param passive Enable passive socket mode
         * \return Information to identify the searched host
         */
        IKNET_EXPORT static ikgen::Result<AddrInfo, std::string> resolve(const std::string& host, uint16_t port, Protocol protocol, bool passive = false);

        /*!
         * \brief Constructor, throws std::runtime_exception if a problem occurs
         * \param host Host name to resolve (can be IP address or domain name for example)
         * \param port Port to reach
         * \param protocol Protocol that will be used to communicate with the host
         * * \param passive Enable passive socket mode
         */
        IKNET_EXPORT AddrInfo(const std::string& host, uint16_t port, Protocol protocol, bool passive = false);
        IKNET_EXPORT ~AddrInfo();

        inline addrinfo* getImpl() { return m_impl; }

    private:

        static constexpr std::pair<int, int> UDP_IDENTIFIERS = std::make_pair(SOCK_DGRAM, IPPROTO_UDP);
        static constexpr std::pair<int, int> TCP_IDENTIFIERS = std::make_pair(SOCK_STREAM, IPPROTO_TCP);

        /*!
         * \brief Gives the system code for the socktype and protocol matching given protocol
         * \param protocol Protocol used for host resolution
         * \return System code for the socktype (first) and protocol (second)
         */
        static const std::pair<int, int>& resolveSystemIdentifiers(Protocol protocol);


        addrinfo* m_impl; // Underlying POSIX addrinfo instance
};

}

#endif // IKNET_ADDR_INFO_HPP
