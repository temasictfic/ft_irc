#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

Channel::Channel(const std::string& ChannelName, Client& op)
{
    _name = ChannelName;
    _topic = "";
    _mode = Default;
    _clientLimit = 10;
    _key = "";
    _operator = &op;
    op._channel = this;
}
Channel::~Channel()
{
}
 
const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

std::vector<class Client> &Channel::getBanned()
{
    return _banned;
}

std::vector<class Client> &Channel::getMembers() 
{
    return _members;
}

void Channel::addMembers(Client &client)
{
    client._channel = this;
   _members.push_back(client);
}

void Channel::addBanned(Client &client)
{
    _banned.push_back(client);
}

void Channel::removeMembers(Client &client)
{
    for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if(it->_nick == client._nick)
        {
            _members.erase(it);
        }
    }
    client._channel = NULL;
}

void Channel::removeBanned(Client &client)
{
        for(std::vector<Client>::iterator it = _banned.begin(); it != _banned.end(); it++)
    {
        if(it->_nick == client._nick)
        {
            _members.erase(it);
        }
    }
}

 Client  *Channel::getOperator() const
 {
    return _operator;
 }

void Channel::setOperator(Client *client)
{
    _operator = client;
}