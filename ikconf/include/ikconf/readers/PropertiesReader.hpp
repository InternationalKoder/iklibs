#ifndef IKCONF_PROPERTIES_READER_HPP
#define IKCONF_PROPERTIES_READER_HPP

#include "BaseReader.hpp"
#include <iklog/Log.hpp>

namespace ikconf
{
    class PropertiesReader : public BaseReader
    {
        public:

            IKCONF_EXPORT PropertiesReader(const Configuration& configuration);

            IKCONF_EXPORT virtual void read(const std::string& filePath);

        private:

            static iklog::Log m_log;
    };
}

#endif // IKCONF_PROPERTIES_READER_HPP
