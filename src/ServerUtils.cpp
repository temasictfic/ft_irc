#include "../inc/Server.hpp"

bool Server::IsExistClient(const std::string &ClientName)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if ((*it)->_nick == ClientName)
            return true;
    }
    return false;
}

bool Server::IsExistChannel(const std::string &ChannelName)
{
    std::map<std::string, class Channel*>::iterator it = _channels.find(ChannelName);
    return (it != _channels.end()) ? true : false;
}

bool Server::IsBannedClient(Client &client, const std::string &ChannelName)
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

bool Server::IsInChannel(Client &client, const std::string &ChannelName)
{
    return (client._channel.find(ChannelName) != client._channel.end()) ? true : false;
}

bool Server::IsOperator(Client &client, const std::string& ChannelName)
{
    return _channels.at(ChannelName)->getOperator()->_nick == client._nick;
}

bool Server::HasChannelKey(const std::string &ChannelName)
{
    return !_channels.at(ChannelName)->getKey().empty();
}

bool Server::IsChannelLimitFull(const std::string &ChannelName)
{
    return _channels.at(ChannelName)->getMembers().size() == _channels.at(ChannelName)->_clientLimit;
}

// Always use under IsExistClient!!!
Client &Server::findClient(const std::string &NickName)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    { 
        if ((*it)->_nick == NickName)
            return (**it);
    }
    return *_clients[-1];
}

int Server::ParamsSizeControl(Client& client, const std::string& Command, std::vector<std::string> params, size_t necessary, size_t optional)
{
    int err = 0;
    if (params.size() < necessary)
        err = -1;
    else if(params.size() > necessary + optional)
        err = 1;
    for (size_t i = 0; i < params.size(); i++)
    {
        if(params[i].empty()){
            err = -1;
            break;
        }
    }
    if (err == -1)
        sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, Command));
    else if (err == 1)
        sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, Command, "Excessive argument is given"));
    return err;
}

enum Prefix Server::PrefixControl(std::string str)
{
    enum Prefix pre = PrefixClient;
    if(!str.empty() && (str[0] == '@') && str[1] == '#')
        pre = PrefixChannelOp;
    else if(!str.empty() && str[0] == '#')
        pre = PrefixChannel;
    return pre;
}

bool Server::PasswordMatched(const std::string& PasswordOrigin, const std::string& PasswordGiven)
{
    return PasswordOrigin == PasswordGiven;
}

const std::string& Server::getPassword() const
{
    return _password;
}
