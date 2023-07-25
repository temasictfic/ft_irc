#include "../inc/Server.hpp"

bool Server::IsExistClient(const std::string &ClientName, const int flag)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (!flag)
        {
            if ((*it)->_nick == ClientName)
                return true;
        }
        else if ((*it)->_username == ClientName)
            return true;
    }
    return false;
}

bool Server::IsExistChannel(const std::string &ChannelName)
{
    std::map<std::string, class Channel*>::iterator it = _channels.find(ChannelName);
    return (it != _channels.end()) ? true : false;
}

bool Server::IsBannedClient(Client &client, const std::string &ChannelName) // channela taşınabilir bu method
{
    std::vector<Client*>::iterator it = _channels.at(ChannelName)->getBanned().begin();
    std::vector<Client*>::iterator end = _channels.at(ChannelName)->getBanned().end();
    for (; it != end; it++)
    {
        std::cout << (*it)->_nick << "$\n";
        if ((*it)->_nick == client._nick)
            return true;
    }
    return false;
}

bool Server::IsInChannel(Client &client, const std::string &ChannelName) // channela taşınabilir bu method
{
    if (client._channel)
        return client._channel->_name == ChannelName;
    else
        return false;
}

/* bool Server::IsOperator(Client &client, const std::string& Nick)
{
    return client._channel->getOperator()->_nick == client._nick;
} */

bool Server::HasChannelKey(const std::string &ChannelName)
{
    std::string NewKey = _channels.at(ChannelName)->getKey();
    return !NewKey.empty();
}

bool Server::IsKeyWrong(const std::string &ChannelName, const std::string &Key)
{
    if (HasChannelKey(ChannelName))
    {
        if (_channels.at(ChannelName)->getKey() == Key)
            return false;
        return true;
    }
    return false;
}

bool Server::IsChannelLimitFull(const std::string &ChannelName)
{
    return _channels.at(ChannelName)->getMembers().size() == _channels.at(ChannelName)->_clientLimit;
}

Client &Server::findClient(const std::string &NickName)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    { 
        if ((*it)->_nick == NickName)
            return (**it);
    }
    return *_clients[-1];
}

int Server::ParamsSizeControl(std::vector<std::string> params, size_t necessary, size_t optional)
{
    if (params.size() < necessary)
        return -1;
    else if(params.size() > necessary + optional)
        return 1;
    for (size_t i = 0; i < params.size(); i++)
    {
        if(params[i].empty())
            return -1;
    }
    return 0;
}

bool Server::PasswordMatched(const std::string& PasswordOrigin, const std::string& PasswordGiven)
{
    return PasswordOrigin == PasswordGiven;
}

const std::string& Server::getPassword() const
{
    return _password;
}