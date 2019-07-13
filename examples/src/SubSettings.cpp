#include "SubSettings.hpp"

SubSettings::SubSettings() :
    ikconf::Configuration(ikconf::Property("my.substring", m_testSubString),
                          ikconf::Property("mysubint", m_testSubInt),
                          ikconf::Property("mysubsubsettings", m_testSubSubSettings))
{}
