/*
    Copyright (C) 2019, InternationalKoder

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

#ifndef IKLOG_FILE_SIZE_HPP
#define IKLOG_FILE_SIZE_HPP

#include "../iklog_export.hpp"
#include <cstdint>

namespace iklog
{
    static constexpr unsigned short UNIT_MULTIPLIER = 1024; // multiplier to go from one unit to the next one

    namespace internal
    {
        /*!
         * \brief Calculates the multiplier to convert any file size unit to bytes
         *
         * M is the level: each level multiplies by 1024
         */
        template<unsigned int M>
        inline constexpr uintmax_t computeFileSizeUnit()
        {
            static_assert(M > 0, "Level of multiplier must be positive");
            return UNIT_MULTIPLIER * computeFileSizeUnit<M - 1>();
        }

        /*!
         * \brief Neutral multiplier for conversion of file sizes to bytes
         */
        template<>
        inline constexpr uintmax_t computeFileSizeUnit<0>()
        {
            return 1;
        }
    }



    /*!
     * \brief Represents the size of a file
     */
    template<unsigned short MULTIPLIER>
    class FileSize
    {
        public:

            FileSize(uintmax_t value) : m_value(value * internal::computeFileSizeUnit<MULTIPLIER>()) {}

            /*!
             * \brief getValueInBytes Returns the file size given in the constructor converted to bytes
             * \return The file size value in bytes
             */
            inline uintmax_t getValueInBytes() const { return m_value; }

        private:

            const uintmax_t m_value; // the value in bytes
    };


    using Bytes     = FileSize<0>; // defines a unit for bytes
    using KiloBytes = FileSize<1>; // defines a unit for kilobytes
    using MegaBytes = FileSize<2>; // defines a unit for megabytes
    using GigaBytes = FileSize<3>; // defines a unit for gigabytes


    inline namespace literals
    {
        Bytes     operator""_b(unsigned long long value)  { return Bytes(value); }     // allows to write bytes like 12_b
        KiloBytes operator""_kb(unsigned long long value) { return KiloBytes(value); } // allows to write kilobytes like 34_kb
        MegaBytes operator""_mb(unsigned long long value) { return MegaBytes(value); } // allows to write megabytes like 56_mb
        GigaBytes operator""_gb(unsigned long long value) { return GigaBytes(value); } // allows to write gigabytes like 78_gb
    }
}

#endif // IKLOG_FILE_SIZE_HPP
