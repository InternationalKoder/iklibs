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
        inline bool getTestCharAsChar() const { return m_testCharAsChar; }
        inline bool getTestCharAsNumber() const { return m_testCharAsNumber; }

    private:

        std::string m_testString;
        int m_testInt;
        float m_testFloat;
        bool m_testBool;
        char m_testCharAsChar;
        char m_testCharAsNumber;
};

#endif // SETTINGS_HPP
