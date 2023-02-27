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

#include "iknet/SocketPoller.hpp"
#include "iknet/systemspec.hpp"
#include <algorithm>

namespace iknet
{

using PollResult = Result<std::optional<Socket*>, std::string>;

const PollResult SocketPoller::poll(const std::chrono::milliseconds& timeout)
{
#ifdef _WIN32
    const int pollResult = WSAPoll(m_pollInfo.data(), static_cast<ULONG>(m_pollInfo.size()), static_cast<INT>(timeout.count()));
#else
    const int pollResult = ::poll(m_pollInfo.data(), static_cast<nfds_t>(m_pollInfo.size()), static_cast<int>(timeout.count()));
#endif

    // Error
    if(pollResult < 0)
        return PollResult::makeFailure("Failed to poll sockets: " + lastNetworkErrorString());

    // Timeout
    if(pollResult == 0)
        return PollResult::makeSuccess();

    for(const priv::PollInfo& pollInfo : m_pollInfo)
    {
        if(pollInfo.revents & POLLRDNORM)
        {
            const auto socketIt = std::find_if(m_sockets.begin(), m_sockets.end(),
                                               [&pollInfo](const Socket* const socket) { return pollInfo.fd == socket->getImpl(); });

            if(socketIt != m_sockets.end())
                return PollResult::makeSuccess(std::make_optional(*socketIt));
        }
    }

    return PollResult::makeSuccess();
}

void SocketPoller::add(Socket& socket)
{
    m_pollInfo.emplace_back(priv::PollInfo
    {
        .fd = socket.getImpl(),
        .events = POLLRDNORM,
        .revents = 0
    });

    m_sockets.emplace_back(&socket);
}
void SocketPoller::remove(const Socket& socket)
{
    std::erase_if(m_pollInfo, [&socket](const priv::PollInfo& pollInfo) { return pollInfo.fd == socket.getImpl(); });
    std::erase(m_sockets, &socket);
}

}
