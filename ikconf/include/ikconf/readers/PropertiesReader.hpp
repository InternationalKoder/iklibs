#ifndef PROPERTIES_READER_HPP
#define PROPERTIES_READER_HPP

#include "BaseReader.hpp"

namespace ikconf
{
    class PropertiesReader : public BaseReader
    {
        public:

            IKCONF_EXPORT PropertiesReader(const Configuration& configuration);

            IKCONF_EXPORT virtual void read(const std::string& filePath);
    };
}

#endif // PROPERTIES_READER_HPP
