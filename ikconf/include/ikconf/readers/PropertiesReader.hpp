#ifndef IKCONF_PROPERTIES_READER_HPP
#define IKCONF_PROPERTIES_READER_HPP

#include "BaseReader.hpp"
#include <iklog/Log.hpp>

namespace ikconf
{
    /*!
     * \brief Reader for .properties configuration files
     */
    class PropertiesReader : public BaseReader
    {
        public:

            /*!
             * \brief Constructor which takes the configuration to valorize
             * \param configuration The configuration that will hold the read values
             */
            IKCONF_EXPORT PropertiesReader(const Configuration& configuration);


            /*!
             * \brief Reads the given .properties file and sets the properties in the configuration (given in the constructor)
             * \param filePath Path to the .properties file to read
             */
            IKCONF_EXPORT virtual void read(const std::string& filePath);

        private:

            static iklog::Log LOG;
    };
}

#endif // IKCONF_PROPERTIES_READER_HPP
