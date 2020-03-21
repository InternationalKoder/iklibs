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

#ifndef IKPARLL_SINGLE_CONSUMER_HPP
#define IKPARLL_SINGLE_CONSUMER_HPP

#include "ConsumerBase.hpp"
#include <thread>

namespace ikparll
{
    /*!
     * \brief A single thread consuming from a queue
     *
     * Template arguments:
     * - T is the type of items to be consumed by the function executed on the threads
     */
    template<typename T>
    class SingleConsumer : public ConsumerBase<T>
    {
        public:

            /*!
             * \brief Constructor taking the function to execute
             * \param function The function that will be executed on queued items by the thread
             */
            SingleConsumer(const std::function<void(T)>& function) :
                ConsumerBase<T>(function)
            {
                init();
            }

            ~SingleConsumer()
            {
                ConsumerBase<T>::stop();
                m_thread.join();
            }

            /*!
             * \brief Initializes the thread
             */
            virtual void init()
            {
                m_thread = std::thread(&SingleConsumer::run, this);
            }

        private:

            std::thread m_thread; // consumer thread
    };
}

#endif // IKPARLL_SINGLE_CONSUMER_HPP
