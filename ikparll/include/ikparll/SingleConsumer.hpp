#ifndef IKPARLL_SINGLE_CONSUMER_HPP
#define IKPARLL_SINGLE_CONSUMER_HPP

#include "ConsumerBase.hpp"

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
                stop();
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
