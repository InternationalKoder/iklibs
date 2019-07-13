#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

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
                if constexpr(std::is_base_of_v<Configuration, T>)
                    m_properties[property.getName()] = static_cast<Configuration*>(property.getValue());
                else
                    m_properties[property.getName()] = property.getValue();

            }

            IKCONF_EXPORT inline std::any getPropertyValue(const std::string& propertyName) const
            {
                return m_properties.at(propertyName);
            }

            IKCONF_EXPORT inline bool checkPropertyExists(const std::string& propertyName) const
            {
                return (m_properties.find(propertyName) != m_properties.end());
            }

        private:

            std::map<std::string, std::any> m_properties;
    };
}

#endif // CONFIGURATION_HPP
