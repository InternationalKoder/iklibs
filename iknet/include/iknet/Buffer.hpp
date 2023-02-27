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

#ifndef IKNET_BUFFER_HPP
#define IKNET_BUFFER_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include "iknet_export.hpp"

namespace iknet
{

/*!
 * \brief A container for data to gather in a single buffer
 */
class Buffer
{
    public:

        /*!
         * \brief Constructor allowing to give an already existing buffer. Uses a new empty buffer otherwise
         * \param buffer The optional buffer to write to or read from
         */
        IKNET_EXPORT Buffer(std::vector<std::byte> buffer = std::vector<std::byte>());

        /*!
         * \brief Adds a char at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(char value);

        /*!
         * \brief Adds an unsigned integer on 8 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(uint8_t value);

        /*!
         * \brief Adds an unsigned integer on 16 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(uint16_t value);

        /*!
         * \brief Adds an unsigned integer on 32 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(uint32_t value);

        /*!
         * \brief Adds an unsigned integer on 64 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(uint64_t value);

        /*!
         * \brief Adds a signed integer on 8 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(int8_t value);

        /*!
         * \brief Adds a signed integer on 16 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(int16_t value);

        /*!
         * \brief Adds a signed integer on 32 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(int32_t value);

        /*!
         * \brief Adds a signed integer on 64 bits at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(int64_t value);

        /*!
         * \brief Adds a float at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(float value);

        /*!
         * \brief Adds a double at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(double value);

        /*!
         * \brief Adds a long double at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(long double value);

        /*!
         * \brief Adds a string at the end of the buffer
         * \param value The value to add
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator<<(std::string_view value);



        /*!
         * \brief Reads a char from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(char& value);

        /*!
         * \brief Reads an unsigned integer on 8 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(uint8_t& value);

        /*!
         * \brief Reads an unsigned integer on 16 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(uint16_t& value);

        /*!
         * \brief Reads an unsigned integer on 32 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(uint32_t& value);

        /*!
         * \brief Reads an unsigned integer on 64 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(uint64_t& value);

        /*!
         * \brief Reads a signed integer on 8 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(int8_t& value);

        /*!
         * \brief Reads a signed integer on 16 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(int16_t& value);

        /*!
         * \brief Reads a signed integer on 32 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(int32_t& value);

        /*!
         * \brief Reads a signed integer on 64 bits from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(int64_t& value);

        /*!
         * \brief Reads a float from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(float& value);

        /*!
         * \brief Reads a double from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(double& value);

        /*!
         * \brief Reads a long double from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(long double& value);

        /*!
         * \brief Reads a string from the buffer
         * \param value [out] The variable that will receive the read value
         * \return A reference to this Buffer object
         */
        IKNET_EXPORT Buffer& operator>>(std::string& value);



        /*!
         * \brief Direct access to the underlying array
         * \return A pointer to the data contained in the buffer
         */
        inline const std::byte* getData() const { return m_buffer.data(); }

        /*!
         * \brief Gives the number of bytes used by elements in the buffer
         * \return The buffer size in bytes
         */
        inline size_t getSize() const { return m_buffer.size(); }

        /*!
         * \brief Tells whether the buffer contains no element
         * \return True if the buffer contains no element
         */
        inline bool isEmpty() const { return m_buffer.empty(); }

    private:

        std::vector<std::byte> m_buffer;
        size_t m_readPosition; // Current position when *reading* the buffer

        /*!
         * \brief Adds a value of any type to the buffer
         * \param value The value to add
         */
        template<typename T>
        void pushValue(const T& value)
        {
            const size_t valueIndex = m_buffer.size();
            m_buffer.resize(valueIndex + sizeof(T));
            std::memcpy(&m_buffer[valueIndex], &value, sizeof(T));
        }

        /*!
         * \brief Reads a value of any type from the buffer at the current position
         * \return The value that has been read
         */
        template<typename T>
        T readValue()
        {
            T value;

            std::memcpy(&value, &m_buffer[m_readPosition], sizeof(T));
            m_readPosition += sizeof(T);

            return value;
        }
};

}

#endif // IKNET_BUFFER_HPP
