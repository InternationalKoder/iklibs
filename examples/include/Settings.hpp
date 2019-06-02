#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <ikconf/Configuration.hpp>

class Settings : public ikconf::Configuration
{
    public:

        Settings();

        inline std::string getResourcesPath() const { return m_resourcesPath; }
        inline int getThreadsNumber() const { return m_threadsNumber; }

    private:

        std::string m_resourcesPath;
        int m_threadsNumber;
};

#endif // SETTINGS_HPP
