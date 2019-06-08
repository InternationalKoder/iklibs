#include "ikconf/readers/BaseReader.hpp"

ikconf::BaseReader::BaseReader(const Configuration& configuration) :
    m_configuration(configuration)
{}

ikconf::BaseReader::~BaseReader()
{}

bool ikconf::BaseReader::tryConvertAndSetProperty(const std::string& name, const std::any& value)
{
    bool added = false;

    added = tryConvertAndSetProperty<bool>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<short int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<unsigned short int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<unsigned int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<long int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<unsigned long int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<long long int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<unsigned long long int>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<char>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<unsigned char>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<float>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<double>(name, value);
    if(added)
        return true;

    added = tryConvertAndSetProperty<long double>(name, value);
    if(added)
        return true;

    // fallback is string
    return tryConvertAndSetProperty<std::string>(name, value);
}
