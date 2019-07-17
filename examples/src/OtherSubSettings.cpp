#include "OtherSubSettings.hpp"

// define the properties in the constructor, like any ikconf configuration
OtherSubSettings::OtherSubSettings() :
    ikconf::Configuration(ikconf::Property("testOtherSubsettingsStr", m_testOtherSubSettingsStr))
{}
