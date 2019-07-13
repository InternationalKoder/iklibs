#ifndef SUB_SETTINGS_HPP
#define SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>
#include "SubSubSettings.hpp"

class SubSettings : public ikconf::Configuration
{
    public:

        SubSettings();

        inline std::string getTestSubString() const { return m_testSubString; }
        inline int getTestSubInt() const { return m_testSubInt; }
        inline SubSubSettings getTestSubSubSettings() const { return m_testSubSubSettings; }

    private:

        std::string m_testSubString;
        int m_testSubInt;
        SubSubSettings m_testSubSubSettings;
};

#endif // SUB_SETTINGS_HPP
