#ifndef IKCONF_CONFIGURATION_EXCEPTION_HPP
#define IKCONF_CONFIGURATION_EXCEPTION_HPP

#include <exception>
#include <string>
#include "ikconf/ikconf_export.hpp"

namespace ikconf
{
    class ConfigurationException : public std::exception
    {
        public:

            ConfigurationException(const std::string& what);

            inline virtual const char* what() const noexcept { return m_what.c_str(); }

        private:

            const std::string m_what;
    };
}

#endif // IKCONF_CONFIGURATION_EXCEPTION_HPP
