#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <ikconf/Configuration.hpp>
#include "SubSettings.hpp"
#include "OtherSubSettings.hpp"

class Settings : public ikconf::Configuration
{
    public:

        Settings();

        inline std::string getTestString() const { return m_testString; }
        inline int getTestInt() const { return m_testInt; }
        inline SubSettings getTestSubSettings() const { return m_testSubSettings; }
        inline float getTestFloat() const { return m_testFloat; }
        inline bool getTestBool() const { return m_testBool; }
        inline OtherSubSettings getTestOtherSubSettings() const { return m_testOtherSubSettings; }
        inline char getTestCharAsChar() const { return m_testCharAsChar; }
        inline char getTestCharAsNumber() const { return m_testCharAsNumber; }
        inline std::vector<int> getTestArray() const { return m_testArray; }
        inline std::vector<std::string> getTestStringArray() const { return m_testStringArray; }

    private:

        std::string m_testString;
        int m_testInt;
        SubSettings m_testSubSettings;
        float m_testFloat;
        bool m_testBool;
        OtherSubSettings m_testOtherSubSettings;
        char m_testCharAsChar;
        char m_testCharAsNumber;
        std::vector<int> m_testArray;
        std::vector<std::string> m_testStringArray;
};

#endif // SETTINGS_HPP
