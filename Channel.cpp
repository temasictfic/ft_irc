#include "Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{

}


std::string Channel::getTopic() const
{
    return _topic;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}
    
std::string Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}
    
unsigned int Channel::getClientLimit() const
{
    return _clientLimit;
}

void Channel::setClientLimit(const unsigned int &clientLimit)
{
     _clientLimit = clientLimit;
}
