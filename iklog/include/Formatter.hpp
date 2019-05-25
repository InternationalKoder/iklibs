#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <string>
#include <map>
#include "Message.hpp"

namespace iklog
{
    class Formatter
    {
        public:

            Formatter(const std::string& format = "%t [%p] %m");

            std::string format(const Message& message) const;

            static std::string getLevel(const Message& message);
            static std::string getLevelPretty(const Message& message);
            static inline std::string getMessage(const Message& message) { return message.getMessage(); }
            static std::string getProgramDuration(const Message& message);
            static std::string getClockTime(const Message& message);

            inline void setFormat(const std::string& format) { m_format = format; }

        private:

            typedef std::string(*getFunc)(const Message&);

            static const std::map<char, getFunc> FORMAT_MAPPING;

            std::string m_format;
    };
}

#endif
