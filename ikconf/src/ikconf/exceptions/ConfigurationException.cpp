#include "ikconf/exceptions/ConfigurationException.hpp"

ikconf::ConfigurationException::ConfigurationException(const std::string& what) :
    m_what(what)
{}
