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

#include "iknet/Buffer.hpp"

#include <cstddef>

namespace iknet
{

#ifdef _WIN32

#include <WinSock2.h>

#else

    #include <arpa/inet.h>

    #ifdef __linux__

    #include <byteswap.h>
    #define iknet_bswap64(x) bswap_64(x)

    #elif defined(__FreeBSD__)

    #include <sys/endian.h>
    #define iknet_bswap64(x) bswap64(x)

    #elif defined(__NetBSD__)

    #include <sys/types.h>
    #include <machine/bswap.h>
    #define iknet_bswap64(x) bswap64(x)

    #elif defined(__OpenBSD__)

    #include <endian.h>
    #define iknet_bswap64(x) swap64(x)

    #elif defined(__APPLE__)

    #include <libkern/OSByteOrder.h>
    #define iknet_bswap64(x) OSSwapInt64(x)

    #endif

    inline uint64_t htonll(uint64_t value)
    {
    #if BYTE_ORDER == BIG_ENDIAN
        return value;
    #else
        return iknet_bswap64(value);
    #endif
    }

    constexpr auto ntohll = htonll;
#endif

Buffer::Buffer(std::vector<std::byte> buffer) :
    m_buffer(std::move(buffer)),
    m_readPosition(0)
{}

Buffer& Buffer::operator<<(char value)
{
    m_buffer.push_back(std::byte(value));
    return *this;
}

Buffer& Buffer::operator<<(uint8_t value)
{
    m_buffer.push_back(std::byte(value));
    return *this;
}

Buffer& Buffer::operator<<(uint16_t value)
{
    pushValue(htons(value));
    return *this;
}

Buffer& Buffer::operator<<(uint32_t value)
{
    pushValue(htonl(value));
    return *this;
}

Buffer& Buffer::operator<<(uint64_t value)
{
    pushValue(htonll(value));
    return *this;
}

Buffer& Buffer::operator<<(int8_t value)
{
    m_buffer.push_back(static_cast<std::byte>(value));
    return *this;
}

Buffer& Buffer::operator<<(int16_t value)
{
    pushValue(static_cast<int16_t>(htons(static_cast<uint16_t>(value))));
    return *this;
}

Buffer& Buffer::operator<<(int32_t value)
{
    pushValue(static_cast<int32_t>(htonl(static_cast<uint32_t>(value))));
    return *this;
}

Buffer& Buffer::operator<<(int64_t value)
{
    pushValue(static_cast<int64_t>(htonll(static_cast<uint64_t>(value))));
    return *this;
}

Buffer& Buffer::operator<<(float value)
{
    pushValue(value);
    return *this;
}

Buffer& Buffer::operator<<(double value)
{
    pushValue(value);
    return *this;
}

Buffer& Buffer::operator<<(long double value)
{
    pushValue(value);
    return *this;
}

Buffer& Buffer::operator<<(std::string_view value)
{
    const size_t strSize = value.size();
    operator<<(static_cast<uint32_t>(strSize));

    const size_t valueIndex = m_buffer.size();
    const size_t strByteSize = strSize * sizeof(std::string::value_type);
    m_buffer.resize(valueIndex + strByteSize);
    std::memcpy(&m_buffer[valueIndex], value.data(), strByteSize);

    return *this;
}

Buffer& Buffer::operator>>(char& value)
{
    value = std::to_integer<char>(m_buffer[m_readPosition]);
    ++m_readPosition;
    return *this;
}

Buffer& Buffer::operator>>(uint8_t& value)
{
    value = std::to_integer<uint8_t>(m_buffer[m_readPosition]);
    ++m_readPosition;
    return *this;
}

Buffer& Buffer::operator>>(uint16_t& value)
{
    const uint16_t networkValue = readValue<uint16_t>();
    value = ntohs(networkValue);
    return *this;
}

Buffer& Buffer::operator>>(uint32_t& value)
{
    const uint32_t networkValue = readValue<uint32_t>();
    value = ntohl(networkValue);
    return *this;
}

Buffer& Buffer::operator>>(uint64_t& value)
{
    const uint64_t networkValue = readValue<uint64_t>();
    value = ntohll(networkValue);
    return *this;
}

Buffer& Buffer::operator>>(int8_t& value)
{
    value = static_cast<int8_t>(m_buffer[m_readPosition]);
    ++m_readPosition;
    return *this;
}

Buffer& Buffer::operator>>(int16_t& value)
{
    const int16_t networkValue = readValue<int16_t>();
    value = static_cast<int16_t>(ntohs(static_cast<uint16_t>(networkValue)));
    return *this;
}

Buffer& Buffer::operator>>(int32_t& value)
{
    const int32_t networkValue = readValue<int32_t>();
    value = static_cast<int32_t>(ntohl(static_cast<uint32_t>(networkValue)));
    return *this;
}

Buffer& Buffer::operator>>(int64_t& value)
{
    const int64_t networkValue = readValue<int64_t>();
    value = static_cast<int64_t>(ntohll(static_cast<uint64_t>(networkValue)));
    return *this;
}

Buffer& Buffer::operator>>(float& value)
{
    value = readValue<float>();
    return *this;
}

Buffer& Buffer::operator>>(double& value)
{
    value = readValue<double>();
    return *this;
}

Buffer& Buffer::operator>>(long double& value)
{
    value = readValue<long double>();
    return *this;
}

Buffer& Buffer::operator>>(std::string& value)
{
    const uint32_t strSize = ntohl(readValue<uint32_t>());
    value.clear();
    value.reserve(strSize);

    const size_t strByteSize = strSize * sizeof(std::string::value_type);

    for(size_t i = 0 ; i < strByteSize ; ++i)
        value.push_back(std::to_integer<char>(m_buffer[m_readPosition + i]));

    m_readPosition += strByteSize;

    return *this;
}

}
