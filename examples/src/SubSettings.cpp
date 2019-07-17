#include "SubSettings.hpp"

// properties in subobjects work exactly the same way as the main configuration class
SubSettings::SubSettings() :
    ikconf::Configuration(ikconf::Property("my.substring", m_testSubString),
                          ikconf::Property("mysubint", m_testSubInt),
                          ikconf::Property("mysubsubsettings", m_testSubSubSettings))
{}
