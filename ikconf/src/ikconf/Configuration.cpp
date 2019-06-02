#include "ikconf/Configuration.hpp"

ikconf::Configuration::Configuration()
{}

void ikconf::Configuration::read()
{
    *std::any_cast<std::string*>(m_properties["resources.path"]) = "path/to/resources";
    *std::any_cast<int*>(m_properties["threads.number"]) = 12;
}
