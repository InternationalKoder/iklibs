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
#include "iknet_export.hpp"

#ifndef _WIN32
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
 * \brief A group of sockets that can be observed to handle incoming data from any of these sockets when it arrives
 */
class SocketPoller
{
    public:

        /*!
         * \brief Waits for incoming data to receive on the group of sockets
         * \param timeout Maximum time to spend waiting for incoming data. Use 0 for unlimited timeout
         * \return In case of success: an optional with a pointer to the first socket with incoming data that has been found, or nothing if the timeout has been reached. An error message in case of failure
         */
        IKNET_EXPORT const Result<std::optional<Socket*>, std::string> poll(const std::chrono::milliseconds& timeout);


        /*!
         * \brief Adds a socket to the group of sockets to poll
         * \param socket The socket to add
         */
        IKNET_EXPORT void add(Socket& socket);

        /*!
         * \brief Removes a socket from the group of sockets to poll
         * \param socket The socket to remove
         */
        IKNET_EXPORT void remove(const Socket& socket);

    private:

        std::vector<priv::PollInfo> m_pollInfo;
        std::vector<Socket*> m_sockets;
};

}

#endif // IKNET_SOCKET_POLLER_HPP
