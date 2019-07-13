#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <string>
#include <vector>
#include <any>

namespace ikconf
{
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
}

#endif // PROPERTY_HPP
