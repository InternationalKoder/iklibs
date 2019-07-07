#include "Settings.hpp"

Settings::Settings() :
    ikconf::Configuration(ikconf::Property("my.string", m_testString),
                          ikconf::Property("myint", m_testInt),
                          ikconf::Property("myfloat", m_testFloat),
                          ikconf::Property("mybool", m_testBool),
                          ikconf::Property("mychar.char", m_testCharAsChar),
                          ikconf::makeSubProperty({"mysubsettings", "my.substring"}, m_testSubString),
                          ikconf::makeSubProperty({"mysubsettings", "mysubint"}, m_testSubInt),
                          ikconf::makeSubProperty({"mysubsettings", "mysubsubsettings", "mysubsubfloat"}, m_testSubSubFloat),
                          ikconf::makeSubProperty({"othersubsettings", "testOtherSubsettingsStr"}, m_testOtherSubSettingsStr))
{
    //addProperty(ikconf::Property("my.string", m_testString));
    //addProperty(ikconf::Property("myint", m_testInt));
    //addProperties(ikconf::Property("my.string", m_testString), ikconf::Property("myint", m_testInt));
}
