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

#ifndef IKNET_SENDER_INFO_HPP
#define IKNET_SENDER_INFO_HPP

#include "iknet_export.hpp"
#include <string>

namespace iknet
{

/*!
 * \brief Stores information about the sender of a received packet
 */
class SenderInfo
{
    public:

        IKNET_EXPORT SenderInfo(std::string host, uint16_t port) :
            m_host(std::move(host)),
            m_port(port)
        {}

        inline std::string_view getHost() const { return m_host; }
        inline uint16_t getPort() const { return m_port; }

    private:

        std::string m_host;
        uint16_t m_port;
};

}

#endif // IKNET_SENDER_INFO_HPP
