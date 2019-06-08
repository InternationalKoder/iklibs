#include "Settings.hpp"

Settings::Settings() :
    ikconf::Configuration(ikconf::Property("my.string", m_testString),
                          ikconf::Property("myint", m_testInt),
                          ikconf::Property("myfloat", m_testFloat),
                          ikconf::Property("mybool", m_testBool),
                          ikconf::Property("mychar.char", m_testCharAsChar),
                          ikconf::Property("mychar.number", m_testCharAsNumber))
{
    //addProperty(ikconf::Property("resources.path", m_resourcesPath));
    //addProperty(ikconf::Property("threads.number", m_threadsNumber));
    //addProperties(ikconf::Property("resources.path", m_resourcesPath), ikconf::Property("threads.number", m_threadsNumber));
}
