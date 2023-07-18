#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

bool Server::IsExistClient(const std::string &ClientName, const int flag)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (!flag)
        {
            if (it->_nick == ClientName)
                return true;
        }
        else if (it->_username == ClientName)
            return true;
    }
    return false;
}

bool Server::IsExistChannel(const std::string &ChannelName)
{
    std::map<std::string, class Channel>::iterator it = _channels.find(ChannelName);
    return (it != _channels.end()) ? true : false;
}

bool Server::IsBannedClient(Client &client, const std::string &ChannelName) // channela taşınabilir bu method
{
    std::vector<Client> &BannedVec = _channels.at(ChannelName).getBanned();
    for (std::vector<Client>::iterator it = BannedVec.begin(); it != BannedVec.end(); it++)
    {
        if (it->_nick == client._nick)
            return false;
    }
    return true;
}

bool Server::IsInChannel(Client &client, const std::string &ChannelName) // channela taşınabilir bu method
{
    std::vector<Client> &MemberVec = _channels.at(ChannelName).getMembers();
    for (std::vector<Client>::iterator it = MemberVec.begin(); it != MemberVec.end(); it++)
    {
        if (it->_nick == client._nick)
            return false;
    }
    return true;
}

bool Server::HasChannelKey(const std::string &ChannelName)
{
    std::string NewKey = _channels.at(ChannelName).getKey();
    return !NewKey.empty();
}

bool Server::IsKeyWrong(const std::string &ChannelName, const std::string &Key)
{
    if (HasChannelKey(ChannelName))
    {
        if (_channels.at(ChannelName).getKey() == Key)
            return false;
        return true;
    }
    return true;
}

bool Server::IsChannelLimitFull(const std::string &ChannelName)
{
    return _channels.at(ChannelName).getMembers().size() == _channels.at(ChannelName)._clientLimit;
}

Client &Server::findClient(const std::string &NickName)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->_nick == NickName)
            return *it;
    }
}

void Server::ChangeMode(enum Mode &mode, const std::string &ModeString)
{
    if (InvalidLetter(ModeString) || InvalidPrefix(ModeString))
    {
        std::map<char, enum Mode> modes = {std::make_pair('i', InviteOnly), std::make_pair('k', KeyChannel), std::make_pair('t', ProtectedTopic)};
        if (ModeString[0] == '+')
            mode = static_cast<enum Mode>(mode | modes.at(ModeString[1]));
        else if (ModeString[0] == '-')
            mode = static_cast<enum Mode>(mode ^ modes.at(ModeString[1]));
    }
    else
        ; // print not valid
}