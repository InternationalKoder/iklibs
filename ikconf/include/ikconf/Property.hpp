#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <string>
#include <vector>
#include <any>
#include <numeric>

namespace ikconf
{
    static constexpr char SUB_PROPERTIES_SEPARATOR = '\x1D';

    template<typename T>
    class Property
    {
        public:

            Property(const std::string& name, T& value) : m_name(name), m_value(&value) {}

            inline std::string getName() const { return m_name; }
            inline T* getValue() const { return m_value; }

        private:

            const std::string m_name;
            T* const m_value;
    };


    // allows to easily build subproperties
    template<typename T>
    Property<T> makeSubProperty(const std::initializer_list<std::string>& path, T& value)
    {
        std::string propName = std::accumulate(path.begin(), path.end(), std::string(), [](std::string& a, const std::string& b) -> std::string
        {
            return a += b + SUB_PROPERTIES_SEPARATOR;
        });
        propName.pop_back();

        return Property(propName, value);
    }
}

#endif // PROPERTY_HPP
