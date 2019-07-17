#ifndef IKCONF_CONFIGURATION_HPP
#define IKCONF_CONFIGURATION_HPP

#include <any>
#include <map>
#include <memory>
#include "Property.hpp"
#include "ikconf_export.hpp"

namespace ikconf
{
    /*!
     * \brief Allows to make a configuration class, holding different properties
     */
    class Configuration
    {
        public:

            /*!
             * \brief Simple constructor that allows to start without any property and set them later
             */
            IKCONF_EXPORT Configuration();


            /*!
             * \brief Constructor directly adding properties to the configuration
             */
            template<typename... T>
            Configuration(const Property<T>&... properties)
            {
                addProperties(properties...);
            }


            /*!
             * \brief Add a list of properties to the configuration
             */
            template<typename T, typename... OTHER>
            inline void addProperties(const Property<T>& property, const Property<OTHER>&... other)
            {
                addProperty(property);
                addProperties(other...);
            }


            /*!
             * \brief Add a single property to the configuration
             */
            template<typename T>
            inline void addProperties(const Property<T>& property)
            {
                addProperty(property);
            }


            /*!
             * \brief Add a single property to the configuration
             */
            template<typename T>
            inline void addProperty(const Property<T>& property)
            {
                if constexpr(std::is_base_of_v<Configuration, T>)
                    m_properties[property.getName()] = static_cast<Configuration*>(property.getValue());
                else
                    m_properties[property.getName()] = property.getValue();

            }


            /*!
             * \brief Gives the value of a stored property
             * \param propertyName The name of the property to read
             * \return The value of the given property
             */
            IKCONF_EXPORT inline std::any getPropertyValue(const std::string& propertyName) const
            {
                return m_properties.at(propertyName);
            }


            /*!
             * \brief Checks if a property is part of the configuration
             * \param propertyName The name of the property to check
             * \return True if the property is in the configuration
             */
            IKCONF_EXPORT inline bool checkPropertyExists(const std::string& propertyName) const
            {
                return (m_properties.find(propertyName) != m_properties.end());
            }

        private:

            std::map<std::string, std::any> m_properties; // List of the stored properties mapped to their names
    };
}

#endif // IKCONF_CONFIGURATION_HPP
