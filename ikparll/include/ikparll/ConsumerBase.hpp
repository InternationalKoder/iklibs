#ifndef IKPARLL_CONSUMER_BASE_HPP
#define IKPARLL_CONSUMER_BASE_HPP

#include <atomic>
#include <functional>
#include <queue>
#include <mutex>

namespace ikparll
{
    template<typename T>
    class ConsumerBase
    {
        public:

            ConsumerBase(const std::function<void(T)>& function) :
                m_function(function),
                m_running(true)
            {}

            /*!
             * \brief Initializes the threads
             */
            virtual void init() = 0;

            /*!
             * \brief Stops the threads
             */
            virtual void stop()
            {
                m_running = false;
                m_condVar.notify_all();
            }

            /*!
             * \brief Adds an item that will be consumed to the queue
             * \param item The item to be consumed by the thread
             */
            virtual void addItem(const T& item)
            {
                {
                    std::lock_guard<std::mutex> lock(m_queueMutex);
                    m_queue.push(item);
                }

                m_condVar.notify_one();
            }

        protected:

            /*!
             * \brief Runs a single thread
             */
            virtual void run()
            {
                while(m_running)
                {
                    std::unique_lock<std::mutex> uniqueLock(m_queueMutex);
                    m_condVar.wait(uniqueLock);

                    while(!m_queue.empty())
                    {
                        // get the item to consume from the queue
                        T itemToConsume = m_queue.front();
                        m_queue.pop();

                        // execute consuming operation without blocking the other threads
                        uniqueLock.unlock();
                        m_function(itemToConsume);
                        uniqueLock.lock();
                    }
                }
            }


            std::atomic<bool> m_running; // allows to stop the thread(s) when set to false

            std::function<void(T)> m_function; // function to be executed by the thread(s)

            std::queue<T> m_queue; // queue containing the items to be consumed by the function executed on the thread(s)
            std::mutex m_queueMutex; // mutex to protect from concurrency on the queue

            std::condition_variable m_condVar; // allows the thread(s) to wait for data
    };
}

#endif // IKPARLL_CONSUMER_BASE_HPP
