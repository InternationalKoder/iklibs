#ifndef JSON_READER_HPP
#define JSON_READER_HPP

#include "BaseReader.hpp"
#include <iklog/Log.hpp>

namespace ikconf
{
    class JsonReader : public BaseReader
    {
        public:

            IKCONF_EXPORT JsonReader(const Configuration& configuration);

            IKCONF_EXPORT virtual void read(const std::string& filePath);

        private:

            static iklog::Log m_log;

            void readObject(std::ifstream& file, Configuration& configuration);
            void readArray(std::ifstream& file, Configuration& configuration, const std::string& propertyName);
            [[noreturn]] void handleUnexpectedCharacter(char character);
            char readCharacter(std::ifstream& file, bool acceptEof = false);

            unsigned int m_lineNumber;
    };
}

#endif // JSON_READER_HPP
