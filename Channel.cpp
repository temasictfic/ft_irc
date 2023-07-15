#include "Channel.hpp"
#include "Client.hpp"

 Channel::Channel()
 {

 }
Channel::Channel(std::string ChannelName)
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
    
size_t Channel::getClientLimit() const
{
    return _clientLimit;
}

void Channel::setClientLimit(const unsigned int &clientLimit)
{
     _clientLimit = clientLimit;
}

std::vector<class Client> &Channel::getBanned()
{
    return _banned;
}
std::vector<class Client> &Channel::getMembers() 
{
    return _members;
}


void Channel::addMembers(Client &Obj)
{
   _members.push_back(Obj);
}