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

#ifndef IKNET_SOCKET_INITIALIZER_HPP
#define IKNET_SOCKET_INITIALIZER_HPP

#include <memory>
#include "iknet_export.hpp"

namespace iknet::priv
{

/*!
 * \brief Internal class handling initialization and cleanup of unerlying socket library when necessary
 */
class SocketInitializer
{
    public:

        inline static void initializeSockets() { static SocketInitializer socketInitializer; }

    private:

        IKNET_EXPORT SocketInitializer();
        IKNET_EXPORT ~SocketInitializer();
};

}

#endif // IKNET_SOCKET_INITIALIZER_HPP
