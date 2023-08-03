#include "../inc/Server.hpp"

Channel::Channel(std::string ChannelName, Client &op)
{
    _name = ChannelName;
    _topic = "";
    _mode = ProtectedTopic;
    _clientLimit = 16;
    _key = "";
    _operator = &op;
    op._channel[ChannelName] = this;
    //_banned = std::vector<class Client*>();
    //_members = std::vector<class Client*>();
    //this->addMembers(op);
}

Channel::Channel(){}

Channel::~Channel()
{
    //delete this;
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
    client._channel.insert(make_pair(_name,this));
   _members.push_back(&client);
}

void Channel::addBanned(Client &client)
{
    client._channel.erase(_name);
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
    client._channel.erase(_name);
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

bool Channel::ChangeModeTwoParams(const std::string& ModeString, const std::map<char,int>& modes)
{
    if (ModeString == "+i" || ModeString == "-i")
    {
        if (ModeString[0] == '+')
            _mode |= modes.at(ModeString[1]);
        else if (ModeString[0] == '-')
            _mode ^= modes.at(ModeString[1]);
    }
    else if (ModeString == "+t" || ModeString == "-t")
    {
        if (ModeString[0] == '+')
            _mode |= modes.at(ModeString[1]);
        else if (ModeString[0] == '-')
            _mode ^= modes.at(ModeString[1]);
    }
    else if (ModeString == "-k" || ModeString == "-l")
    {
        _mode ^= modes.at(ModeString[1]);
        if (ModeString[1] == 'k')
            _key = "";
        else if (ModeString[1] == 'l')
            _clientLimit = 16;
    }
    else
        return false;
    return true;
}

bool Channel::ChangeModeThreeParams(const std::string& ModeString, const std::string& ModeArg, const std::map<char,int>& modes)
{
    if (ModeString == "+l")
    {
        size_t limit = strtol(ModeArg.c_str(), NULL, 10);
        if (limit > _members.size() && limit <= 16)
        {
            _clientLimit = limit;
            _mode |= modes.at(ModeString[1]);
        }
        else
            return false;
    }
    else if (ModeString == "+k")
    {
        if(InvalidPassword(ModeArg))
            return false;
        _key = ModeArg;
        _mode |= modes.at(ModeString[1]);
    }
    return true;
}

bool Channel::ChangeBannedMode(Client &banned, const std::string &ModeString, bool isbanned)
{
    if (ModeString == "+b")
    {
        if(getOperator()->_nick != banned._nick && !isbanned)
        {
            removeMembers(banned);
            addBanned(banned);
            return true;
        }
    }
    else if (ModeString == "-b")
    {
        if(getOperator()->_nick != banned._nick && isbanned)
        {
            removeBanned(banned);  
            return true;
        }
    }
    return false;
}

