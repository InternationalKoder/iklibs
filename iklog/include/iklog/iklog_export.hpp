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


#ifndef IKLOG_EXPORT_H
#define IKLOG_EXPORT_H

#ifdef IKLOG_BUILT_AS_STATIC
#  define IKLOG_EXPORT
#  define IKLOG_NO_EXPORT
#else
#  ifndef IKLOG_EXPORT
#    ifdef iklog_EXPORTS
        /* We are building this library */
#      define IKLOG_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define IKLOG_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef IKLOG_NO_EXPORT
#    define IKLOG_NO_EXPORT 
#  endif
#endif

#ifndef IKLOG_DEPRECATED
#  define IKLOG_DEPRECATED __declspec(deprecated)
#endif

#ifndef IKLOG_DEPRECATED_EXPORT
#  define IKLOG_DEPRECATED_EXPORT IKLOG_EXPORT IKLOG_DEPRECATED
#endif

#ifndef IKLOG_DEPRECATED_NO_EXPORT
#  define IKLOG_DEPRECATED_NO_EXPORT IKLOG_NO_EXPORT IKLOG_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IKLOG_NO_DEPRECATED
#    define IKLOG_NO_DEPRECATED
#  endif
#endif

#endif /* IKLOG_EXPORT_H */
