#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <ikconf/Configuration.hpp>
#include "SubSettings.hpp"
#include "OtherSubSettings.hpp"

/*!
 * \brief Example of the implementation of a configuration with ikconf
 *
 * Shows how to implement a configuration (ikconf) :
 * - inherit the ikconf::Configuration class
 * - you can add other subclasses of ikconf::Configuration to group some properties in subobjects
 * - if you want to have lists in your configuration, you must use std::vector so that ikconf can put the values
 * - the properties are defined in the constructor
 */
class Settings : public ikconf::Configuration
{
    public:

        Settings();

        // getters for the properties
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

        // properties to valorize
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
