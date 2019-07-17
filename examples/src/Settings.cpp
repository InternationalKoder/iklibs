#include "Settings.hpp"

// you can define the properties by giving them in the ikconf::Configuration constructor
// the link between the name of the property in the file and the class member is done through the ikconf::Property class
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
    // alternatively, you can also define the properties by calling the addProperty and addProperties methods:

    //addProperty(ikconf::Property("my.string", m_testString));
    //addProperties(ikconf::Property("my.string", m_testString), ikconf::Property("myint", m_testInt));
}
