#ifndef IKCONF_CONFIGURATION_EXCEPTION_HPP
#define IKCONF_CONFIGURATION_EXCEPTION_HPP

#include <exception>
#include <string>
#include "ikconf/ikconf_export.hpp"

namespace ikconf
{
    /*!
     * \brief Exception that is thrown when a problem occurs while building a configuration
     */
    class ConfigurationException : public std::exception
    {
        public:

            /*!
             * \brief Constructor which takes the description of the encountered problem
             * \param what Description of the problem
             */
            ConfigurationException(const std::string& what);


            /*!
             * \brief Gives the description of the error
             * \return A string describing the problem
             */
            inline virtual const char* what() const noexcept { return m_what.c_str(); }

        private:

            const std::string m_what;
    };
}

#endif // IKCONF_CONFIGURATION_EXCEPTION_HPP
