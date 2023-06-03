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

#ifndef IKNET_UDP_REC_LENGTH_HPP
#define IKNET_UDP_REC_LENGTH_HPP

#include "SenderInfo.hpp"

namespace iknet
{

/*!
 * \brief Stores the length of received content and information about its sender
 */
class UdpRecLength
{
    public:

        UdpRecLength(size_t length, std::string senderHost, uint16_t senderPort) :
            m_length(length),
            m_sender(std::move(senderHost), senderPort)
        {}

        UdpRecLength(size_t length, SenderInfo sender) :
            m_length(length),
            m_sender(std::move(sender))
        {}

        inline size_t getLength() const { return m_length; }
        inline const SenderInfo& getSender() const { return m_sender; }

    private:

        size_t m_length;
        SenderInfo m_sender;
};

}

#endif // IKNET_UDP_REC_LENGTH_HPP
