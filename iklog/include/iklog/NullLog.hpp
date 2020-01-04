/*
    Copyright (C) 2020, InternationalKoder

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

#ifndef IKLOG_NULL_LOG_HPP
#define IKLOG_NULL_LOG_HPP

#include "Log.hpp"
#include "iklog_export.hpp"

namespace iklog
{
    /*!
     * \brief Singleton for a iklog::Log that does nothing
     */
    class NullLog : public Log
    {
        public:

            NullLog(const NullLog&) = delete;
            NullLog(const NullLog&&) = delete;

            NullLog& operator=(const NullLog&) = delete;
            NullLog& operator=(const NullLog&&) = delete;

            /*!
             * \brief Gives the singleton instance
             * \return The singleton instance
             */
            IKLOG_EXPORT static inline NullLog& getInstance() { return m_nullLog; }

            /*!
             * \brief Logs nothing
             */
            IKLOG_EXPORT inline virtual void log(Level, const std::string&) const override {}

        private:

            NullLog();

            static NullLog m_nullLog;
    };
}

#endif // IKLOG_NULL_LOG_HPP
