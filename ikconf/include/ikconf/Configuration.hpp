#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <initializer_list>
#include <any>
#include <map>
#include <memory>
#include "Property.hpp"
#include "ikconf_export.hpp"

namespace ikconf
{
    class Configuration
    {
        public:

            IKCONF_EXPORT Configuration();

            template<typename... T>
            Configuration(const Property<T>&... properties)
            {
                addProperties(properties...);
            }

            template<typename T, typename... OTHER>
            inline void addProperties(const Property<T>& property, const Property<OTHER>&... other)
            {
                addProperty(property);
                addProperties(other...);
            }

            template<typename T>
            inline void addProperties(const Property<T>& property)
            {
                addProperty(property);
            }

            template<typename T>
            inline void addProperty(const Property<T>& property)
            {
                m_properties[property.getName()] = property.getValue();
            }

            IKCONF_EXPORT void read();

        private:

            std::map<std::string, std::any> m_properties;
    };
}

#endif // CONFIGURATION_HPP
