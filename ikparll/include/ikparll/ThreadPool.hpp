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
