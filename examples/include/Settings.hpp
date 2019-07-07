#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <ikconf/Configuration.hpp>

class Settings : public ikconf::Configuration
{
    public:

        Settings();

        inline std::string getTestString() const { return m_testString; }
        inline int getTestInt() const { return m_testInt; }
        inline float getTestFloat() const { return m_testFloat; }
        inline bool getTestBool() const { return m_testBool; }
        inline char getTestCharAsChar() const { return m_testCharAsChar; }
        inline char getTestCharAsNumber() const { return m_testCharAsNumber; }
        inline std::string getTestSubString() const { return m_testSubString; }
        inline int getTestSubInt() const { return m_testSubInt; }
        inline float getTestSubSubFloat() const { return m_testSubSubFloat; }
        inline std::string getTestOtherSubSettingsStr() const { return m_testOtherSubSettingsStr; }

    private:

        std::string m_testString;
        int m_testInt;
        float m_testFloat;
        bool m_testBool;
        char m_testCharAsChar;
        char m_testCharAsNumber;
        std::string m_testSubString;
        int m_testSubInt;
        float m_testSubSubFloat;
        std::string m_testOtherSubSettingsStr;
};

#endif // SETTINGS_HPP
