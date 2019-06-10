#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <string>
#include <map>
#include "Message.hpp"
#include "iklog_export.hpp"

namespace iklog
{
    class Formatter
    {
        public:

            IKLOG_EXPORT Formatter(const std::string& format = "%t %L [%p] %m");

            IKLOG_EXPORT std::string format(const Message& message) const;

            IKLOG_EXPORT static inline std::string getLogName(const Message& message) { return message.getLogName(); }
            IKLOG_EXPORT static std::string getLevel(const Message& message);
            IKLOG_EXPORT static std::string getLevelPretty(const Message& message);
            IKLOG_EXPORT static inline std::string getMessage(const Message& message) { return message.getMessage(); }
            IKLOG_EXPORT static std::string getProgramDuration(const Message& message);
            IKLOG_EXPORT static std::string getClockTime(const Message& message);

            IKLOG_EXPORT inline void setFormat(const std::string& format) { m_format = format; }

        private:

            typedef std::string(*getFunc)(const Message&);

            static const std::map<char, getFunc> FORMAT_MAPPING;

            std::string m_format;
    };
}

#endif
