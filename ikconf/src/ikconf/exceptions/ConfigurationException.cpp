#include "ikconf/exceptions/ConfigurationException.hpp"

namespace ikconf
{
    ikconf::ConfigurationException::ConfigurationException(const std::string& what) :
        m_what(what)
    {}
}
