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

const std::string &Channel::getTopic() const
{
    return _topic;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}
    
const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}
    
const size_t Channel::getClientLimit() const
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

void Channel::addBanned(Client &Obj)
{
    _banned.push_back(Obj);
}

void Channel::removeMembers(Client &Obj)
{
    for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if(it->getNick() == Obj.getNick())
        {
            _members.erase(it);
        }
    }
}

void Channel::removeBanned(Client &Obj)
{
        for(std::vector<Client>::iterator it = _banned.begin(); it != _banned.end(); it++)
    {
        if(it->getNick() == Obj.getNick())
        {
            _members.erase(it);
        }
    }
}

 Client  *Channel::getOperator() const
 {
    return _operator;
 }

void Channel::setOperator(Client *Obj)
{
    _operator = Obj;
}