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

#ifndef IKNET_SOCKET_POLLER_HPP
#define IKNET_SOCKET_POLLER_HPP

#include <vector>
#include <optional>
#include <chrono>
#include "Socket.hpp"
#include "Result.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <poll.h>
#endif

namespace iknet
{

namespace priv
{
#ifdef _WIN32
    using PollInfo = WSAPOLLFD;
#else
    using PollInfo = pollfd;
#endif
}

/*!
 * \brief A group of sockets that can be observed to handle incoming data from any of these sockets when it arrives.
 *
 * By default the SocketPoller is able to contain different types of socket. However it is possible to give a template argument to specify
 * that only one type of socket can be held. This allows the poll method to return a pointer to the concrete socket type.
 */
template<typename T = Socket>
requires std::is_base_of_v<Socket, T>
class SocketPoller
{
    public:

        /*!
         * \brief Waits for incoming data to receive on the group of sockets
         * \param timeout Maximum time to spend waiting for incoming data. Use 0 for unlimited timeout
         * \return In case of success: an optional with a pointer to the first socket with incoming data that has been found,
         * or nothing if the timeout has been reached. An error message in case of failure
         */
        const Result<std::optional<T*>, std::string> poll(const std::chrono::milliseconds& timeout)
        {
#ifdef _WIN32
            const int pollResult = WSAPoll(m_pollInfo.data(), static_cast<ULONG>(m_pollInfo.size()), static_cast<INT>(timeout.count()));
#else
            const int pollResult = ::poll(m_pollInfo.data(), static_cast<nfds_t>(m_pollInfo.size()), static_cast<int>(timeout.count()));
#endif

            // Error
            if(pollResult < 0)
                return Result<std::optional<T*>, std::string>::makeFailure("Failed to poll sockets: " + lastNetworkErrorString());

            // Timeout
            if(pollResult == 0)
                return Result<std::optional<T*>, std::string>::makeSuccess();

            for(const priv::PollInfo& pollInfo : m_pollInfo)
            {
                if(pollInfo.revents & POLLRDNORM)
                {
                    const auto socketIt = std::find_if(m_sockets.begin(), m_sockets.end(),
                                                       [&pollInfo](const Socket* const socket) { return pollInfo.fd == socket->getImpl(); });

                    if(socketIt != m_sockets.end())
                        return Result<std::optional<T*>, std::string>::makeSuccess(std::make_optional(*socketIt));
                }
            }

            return Result<std::optional<T*>, std::string>::makeSuccess();
        }


        /*!
         * \brief Adds a socket to the group of sockets to poll
         * \param socket The socket to add
         */
        void add(T& socket)
        {
            m_pollInfo.emplace_back(priv::PollInfo
            {
                .fd = socket.getImpl(),
                .events = POLLRDNORM,
                .revents = 0
            });

            m_sockets.emplace_back(&socket);
        }

        /*!
         * \brief Removes a socket from the group of sockets to poll
         * \param socket The socket to remove
         */
        void remove(const T& socket)
        {
            std::erase_if(m_pollInfo, [&socket](const priv::PollInfo& pollInfo) { return pollInfo.fd == socket.getImpl(); });
            std::erase(m_sockets, &socket);
        }

    private:

        std::vector<priv::PollInfo> m_pollInfo;
        std::vector<T*> m_sockets;
};

}

#endif // IKNET_SOCKET_POLLER_HPP
