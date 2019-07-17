#ifndef SUB_SUB_SETTINGS_HPP
#define SUB_SUB_SETTINGS_HPP

#include <ikconf/Configuration.hpp>

/*!
 * \brief Allows to test subobjects within other subobjects in a configuration (ikconf)
 */
class SubSubSettings : public ikconf::Configuration
{
    public:

        SubSubSettings();

        inline float getTestSubSubFloat() const { return m_testSubSubFloat; }

    private:

        float m_testSubSubFloat;
};

#endif // SUB_SUB_SETTINGS_HPP
