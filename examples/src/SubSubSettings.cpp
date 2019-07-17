#include "SubSubSettings.hpp"

// properties in subobjects work exactly the same way as the main configuration class
SubSubSettings::SubSubSettings() :
    ikconf::Configuration(ikconf::Property("mysubsubfloat", m_testSubSubFloat))
{}
