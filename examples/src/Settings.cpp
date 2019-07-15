#include "Settings.hpp"

Settings::Settings() :
    ikconf::Configuration(ikconf::Property("my.string", m_testString),
                          ikconf::Property("myint", m_testInt),
                          ikconf::Property("myfloat", m_testFloat),
                          ikconf::Property("mybool", m_testBool),
                          ikconf::Property("mychar.char", m_testCharAsChar),
                          ikconf::Property("mysubsettings", m_testSubSettings),
                          ikconf::Property("othersubsettings", m_testOtherSubSettings),
                          ikconf::Property("myarray", m_testArray),
                          ikconf::Property("mystringarray", m_testStringArray))
{
    //addProperty(ikconf::Property("my.string", m_testString));
    //addProperty(ikconf::Property("myint", m_testInt));
    //addProperties(ikconf::Property("my.string", m_testString), ikconf::Property("myint", m_testInt));
}
