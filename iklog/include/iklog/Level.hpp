#ifndef IKLOG_LEVELS_HPP
#define IKLOG_LEVELS_HPP

namespace iklog
{
    /*!
     * \brief Defines the logging levels
     */
    enum Level
    {
        INFO = 0x0001,
        DEBUG = 0x0010,
        WARNING = 0x0100,
        ERROR = 0x1000
    };
}

#endif // IKLOG_LEVELS_HPP

