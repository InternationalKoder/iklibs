/*
    Copyright (C) 2019, 2020, 2023, InternationalKoder

    This file is part of IKLibs.

    IKLibs is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IKLibs is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with IKLibs.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef IKCONF_CONFIGURATION_HPP
#define IKCONF_CONFIGURATION_HPP

#include <any>
#include <map>
#include <memory>
#include <stdexcept>
#include "Property.hpp"

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
        Configuration() = default;

        virtual ~Configuration() = default;


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
        inline const std::any& getPropertyValue(const std::string& propertyName) const
        {
            return m_properties.at(propertyName);
        }


        /*!
         * \brief Checks if a property is part of the configuration
         * \param propertyName The name of the property to check
         * \return True if the property is in the configuration
         */
        inline bool checkPropertyExists(const std::string& propertyName) const
        {
            return (m_properties.find(propertyName) != m_properties.end());
        }


        /*!
         * \brief For list Configuration only: creates a new element in the list and returns it
         * \return A pointer to the new element in the list
         */
        virtual inline Configuration* newListItem() { throw std::runtime_error("This configuration element is not a list"); }

    private:

        std::map<std::string, std::any> m_properties; // List of the stored properties mapped to their names
};

}

#endif // IKCONF_CONFIGURATION_HPP
