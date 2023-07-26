#include "../inc/Server.hpp"

Channel::Channel(std::string ChannelName, Client &op)
{
    _name = ChannelName;
    _topic = "";
    _mode = Default;
    _clientLimit = 10;
    _key = "";
    _operator = &op;
    op._channel = this;
    //_banned = std::vector<class Client*>();
    //_members = std::vector<class Client*>();
    //this->addMembers(op);
}

Channel::Channel(){}

Channel::~Channel()
{
    _operator->_channel = NULL;

    _banned.clear();
    for(std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
        (*it)->_channel = NULL;
    _members.clear();
}
 
const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

std::vector<class Client*>& Channel::getBanned()
{
    return _banned;
}

std::vector<class Client*>& Channel::getMembers() 
{
    return _members;
}

void Channel::addMembers(Client &client)
{
    client._channel = this;
   _members.push_back(&client);
}

void Channel::addBanned(Client &client)
{
    client._channel = NULL;
     _banned.push_back(&client);
}

void Channel::removeMembers(Client &client)
{
    for(std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if((*it)->_nick == client._nick)
        {
            _members.erase(it);
            break;
        }
    }
    client._channel = NULL;
}

void Channel::removeBanned(Client &client)
{
    for(std::vector<Client*>::iterator it = _banned.begin(); it != _banned.end(); it++)
    {
        if((*it)->_nick == client._nick)
        {
            _banned.erase(it);
            return ;
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

bool ChangeMode(Client &client, const std::vector<std::string> &params, std::map<char,int> modes)
{
    if(params.size() == 3 && params[1][1] != 'k')
            return false;
    if (modes.find(params[1][1]) != modes.end())
    {
        if (params[1][0] == '+')
        {
            client._channel->_mode |= modes.at(params[1][1]);
            if(params[1][1] == 'k' && params.size() == 3)
                client._channel->setKey(params[2]);
            return true;
        }
        else if (params[1][0] == '-')
        {
            client._channel->_mode ^= modes.at(params[1][1]);
            if(params[1][1] == 'k' && params.size() == 3)
                client._channel->setKey("");
            return true;
        }
    }
    return false;
}
