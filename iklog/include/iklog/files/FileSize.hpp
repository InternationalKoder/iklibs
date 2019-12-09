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
    /*!
     * \brief Represents the size of a file
     */
    template<unsigned short MULTIPLIER>
    class FileSize
    {
        public:

            FileSize(uintmax_t value) : m_value(value * computeUnit<MULTIPLIER>()) {}

            inline uintmax_t getValueInBytes() const { return m_value; }

        private:

            template<unsigned int M>
            inline uintmax_t computeUnit()
            {
                static_assert(M > 0, "Level of multiplier must be positive");
                return UNIT_MULTIPLIER * computeUnit<M - 1>();
            }

            template<>
            inline uintmax_t computeUnit<0>()
            {
                return 1;
            }


            static constexpr unsigned short UNIT_MULTIPLIER = 1024; // multiplier to go from one unit to the next one


            const uintmax_t m_value; // the value in bytes
    };


    using Bytes     = FileSize<0>;
    using KiloBytes = FileSize<1>;
    using MegaBytes = FileSize<2>;
    using GigaBytes = FileSize<3>;


    inline namespace literals
    {
        Bytes     operator""_b(uintmax_t value)  { return Bytes(value); }
        KiloBytes operator""_kb(uintmax_t value) { return KiloBytes(value); }
        MegaBytes operator""_mb(uintmax_t value) { return MegaBytes(value); }
        GigaBytes operator""_gb(uintmax_t value) { return GigaBytes(value); }
    }
}

#endif // IKLOG_FILE_SIZE_HPP
