#ifndef OTHER_SUB_SETTINGS_HPP
#define OTHER_SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>

class OtherSubSettings : public ikconf::Configuration
{
    public:

        OtherSubSettings();

        inline std::string getTestOtherSubSettingsStr() const { return m_testOtherSubSettingsStr; }

    private:

        std::string m_testOtherSubSettingsStr;
};

#endif // OTHER_SUB_SETTINGS_HPP
