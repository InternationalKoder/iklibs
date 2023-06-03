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

#ifndef IKNET_UDP_REC_BUFFER_HPP
#define IKNET_UDP_REC_BUFFER_HPP

#include "Buffer.hpp"
#include "SenderInfo.hpp"

namespace iknet
{

/*!
 * \brief Stores received content and information about its sender
 */
class UdpRecBuffer
{
    public:

        UdpRecBuffer(Buffer buffer, std::string senderHost, uint16_t senderPort) :
            m_buffer(std::move(buffer)),
            m_sender(std::move(senderHost), senderPort)
        {}

        UdpRecBuffer(Buffer buffer, SenderInfo sender) :
            m_buffer(std::move(buffer)),
            m_sender(std::move(sender))
        {}

        inline Buffer& getBuffer() { return m_buffer; }
        inline const SenderInfo& getSender() const { return m_sender; }

    private:

        Buffer m_buffer;
        SenderInfo m_sender;
};

}

#endif // IKNET_UDP_REC_BUFFER_HPP
