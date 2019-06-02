#include "Settings.hpp"

Settings::Settings() :
    ikconf::Configuration(ikconf::Property("resources.path", m_resourcesPath),
                          ikconf::Property("threads.number", m_threadsNumber))
{
    //addProperty(ikconf::Property("resources.path", m_resourcesPath));
    //addProperty(ikconf::Property("threads.number", m_threadsNumber));
    //addProperties(ikconf::Property("resources.path", m_resourcesPath), ikconf::Property("threads.number", m_threadsNumber));
}
