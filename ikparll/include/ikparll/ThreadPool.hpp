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

#ifndef IKPARLL_THREAD_POOL_HPP
#define IKPARLL_THREAD_POOL_HPP

#include <array>
#include "ConsumerBase.hpp"

namespace ikparll
{
    /*!
     * \brief A fixed size thread pool consuming from a queue
     *
     * Template arguments:
     * - T is the type of items to be consumed by the function executed on the threads
     * - S is the size of the thread pool
     */
    template<typename T, unsigned int S>
    class ThreadPool : public ConsumerBase<T>
    {
        public:

            static_assert(S > 0, "Cannot have a pool of 0 threads");
            static_assert(S != 1, "Please use SingleConsumer for a single-thread pool");

            /*!
             * \brief Constructor taking the function to execute
             * \param function The function that will be executed on queued items by the threads
             */
            ThreadPool(const std::function<void(T)>& function) :
                ConsumerBase<T>(function)
            {
                init();
            }

            ~ThreadPool()
            {
                stop();

                for(std::thread& thread : m_threads)
                    thread.join();
            }

            /*!
             * \brief Initializes the threads
             */
            virtual void init()
            {
                for(std::thread& thread : m_threads)
                    thread = std::thread(&ThreadPool::run, this);
            }

        private:

            std::array<std::thread, S> m_threads; // thread pool
    };
}

#endif // IKPARLL_THREAD_POOL_HPP
