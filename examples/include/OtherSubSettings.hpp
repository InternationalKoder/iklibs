#ifndef OTHER_SUB_SETTINGS_HPP
#define OTHER_SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>

/*!
 * \brief Allows to test multiple subobjects in a configuration (ikconf)
 */
class OtherSubSettings : public ikconf::Configuration
{
    public:

        OtherSubSettings();

        inline std::string getTestOtherSubSettingsStr() const { return m_testOtherSubSettingsStr; }

    private:

        std::string m_testOtherSubSettingsStr;
};

#endif // OTHER_SUB_SETTINGS_HPP
