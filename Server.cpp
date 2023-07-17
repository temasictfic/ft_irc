#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Server::Server(/* args */)
{
}

Server::~Server()
{
}

void Server::Pass(size_t idx, const std::string &Password)
{
    if (!InvalidPassword(Password))
    {
        switch (_clients[idx]._status) // _status
        {
        case RegistrationState::None:
            _clients[idx].setPassword(Password);
            _clients[idx]._status = RegistrationState::PassRegistered;
            // password assigned yazdır?;
            break;
        case RegistrationState::UsernameRegistered:
            _clients[idx].setPassword(Password);
            // password changed yazdır?;
            break;
        }
    }
    // Invalid Password hatası
}
void Server::Nick(size_t idx, const std::string &NickName)
{
    if (InvalidLetter(NickName) || InvalidPrefix(NickName))
        // hata durumu;
        ;
    else if (IsExistClient(NickName, 0))
        // hata durumu
        ;
    switch (_clients[idx]._status) // _status
    {
    case RegistrationState::PassRegistered:
        _clients[idx]._nick = ToLowercase(NickName);
        _clients[idx]._status = RegistrationState::NickRegistered;
        // nick assigned yazdır?;
        break;
    case RegistrationState::NickRegistered | RegistrationState::UsernameRegistered: //çalışmayabilir
        _clients[idx]._nick = ToLowercase(NickName);
        // nick changed yazdır?;
        break;
    }
}

void Server::User(size_t idx, const std::string &Username)
{
    if (InvalidLetter(Username) || InvalidPrefix(Username))
        // hata durumu;
        ;
    switch (_clients[idx]._status) // _status
    {
    case RegistrationState::NickRegistered:
        _clients[idx]._username = ToLowercase(Username);
        _clients[idx]._realname = "";
        _clients[idx]._status = RegistrationState::UsernameRegistered;
        // username assigned yazdır?;
        break;
    case RegistrationState::UsernameRegistered:
        _clients[idx]._username = ToLowercase(Username);
        // username changed yazdır?;
        break;
    }
}

void Server::User(size_t idx, std::string &Username, const std::string &Realname)
{
    if (InvalidLetter(Username) || InvalidPrefix(Username) || InvalidLetter(Realname) || InvalidPrefix(Realname))
        // hata durumu;
        ;
    switch (_clients[idx]._status) // _status
    {
    case RegistrationState::NickRegistered:
        _clients[idx]._username = Username;
        _clients[idx]._realname = Realname;
        _clients[idx]._status = RegistrationState::UsernameRegistered;
        // username assigned yazdır?;
        break;
    case RegistrationState::UsernameRegistered:
        _clients[idx]._username = Username;
        _clients[idx]._realname = Realname;
        // username and realname changed yazdır?;
        break;
    }
}

void Server::Ping(size_t idx)
{
    sendMessageToClient(idx, "Ping");
}
void Server::Quit(size_t idx, const std::string &Message)
{
    // print client nickname quited msg
    // remove client all the members list of channels joined
    // remove from clients.
    close(_clients[idx].getSocketFd());
}
void Server::Join(size_t idx, const std::string &ChannelName)
{
    if (IsExistChannel(ChannelName))
    {
        if (IsInChannel(idx, ChannelName))
        {
            // print client already in channel msg;
        }

        else if (IsBannedClient(idx, ChannelName))
        {
            // print client cannot join bcs in banned list of channel msg;
        }
        else if (IsChannelLimitFull(ChannelName))
        {
            // Channel is full print error
        }
        else if (HasChannelKey(ChannelName))
        {
            // if has key key error
        }
        else
        {
            _channels[ChannelName].addMembers(_clients[idx]);
        }
    }
    else
    {
        if (InvalidLetter(ChannelName) || InvalidPrefix(ChannelName))
        {
            // print invalid ChannelName error
        }
        else
        {
            Channel newish(ChannelName);
            _channels.insert(std::pair<std::string, Channel>(ChannelName, newish));
        }
    }
}
void Server::Join(size_t idx, const std::string &ChannelName, const std::string &ChannelKey)
{
    if (IsExistChannel(ChannelName))
    {
        if (IsInChannel(idx, ChannelName))
        {
            // print client already in channel msg;
        }

        else if (IsBannedClient(idx, ChannelName))
        {
            // print client cannot join bcs in banned list of channel msg;
        }
        else if (IsChannelLimitFull(ChannelName))
        {
            // Channel is full
        }
        else if (IsKeyWrong(ChannelName, ChannelKey))
        {
            // if key wrong print error
        }
        else
        {
            _channels[ChannelName].addMembers(_clients[idx]);
        }
    }
    else
    {
        if (InvalidLetter(ChannelName) || InvalidPrefix)
        {
            // print invalid ChannelName error
        }
        else
        {
            Channel newish(ChannelName);
            _channels.insert(std::pair<std::string, Channel>(ChannelName, newish));
            _channels[ChannelName].setKey(ChannelKey);
        }
    }
}
void Server::Part(size_t idx, const std::string &ChannelName, const std::string &Reason)
{
    // print reason
    _channels[ChannelName].removeMembers(_clients[idx]);
}
void Server::Topic(size_t idx, const std::string &ChannelName, const std::string &TopicName)
{
    if (InvalidPrefix(TopicName) || InvalidLetter(TopicName))
        // print error doesnt allow this letter
        if (IsExistChannel(ChannelName) && IsInChannel(idx, ChannelName))
        {
            if (_channels[ChannelName]._mode == Mode::ProtectedTopic && _channels[ChannelName].getOperator()->_nick == _clients[idx]._nick)
                _channels[ChannelName]._topic = TopicName;
            else if (_channels[ChannelName]._mode == Mode::ProtectedTopic)
            {
                // print error
            }
            else
                _channels[ChannelName]._topic = TopicName;;
        }
}
void Server::Names(const std::string &ChannelName)
{
    if (IsExistChannel(ChannelName))
    {
        std::cout << '@' << _channels[ChannelName].getOperator()->_nick << '\n';
        for (std::vector<Client>::iterator it = _channels[ChannelName].getMembers().begin(); it != _channels[ChannelName].getMembers().end(); ++it)
        {
            std::cout << it->_nick << "\n";
        }
    }
}
void Server::Invite(size_t idx, const std::string &ChannelName, const std::string &NickName)
{
    if (!IsExistClient(NickName, 0))
        // print error
        // check there is a man with this nick in clients
        if (IsExistChannel(ChannelName) && IsInChannel(idx, ChannelName))
        {
            if (_channels[ChannelName]._mode == Mode::InviteOnly && _channels[ChannelName].getOperator()->_nick == _clients[idx]._nick)
                _channels[ChannelName].addMembers(findClient(NickName));
            else if (_channels[ChannelName]._mode == Mode::InviteOnly)
            {
                // print error
            }
            else
                _channels[ChannelName].addMembers(findClient(NickName));
        }
}
void Server::Mode(size_t idx, const std::string &ChannelName, const std::string &ModeString)
{
    if (IsExistChannel(ChannelName) && _channels[ChannelName].getOperator()->_nick == _clients[idx]._nick)
    {
        if (ModeString.size() == 2)
        {
            ChangeMode(_channels[ChannelName]._mode, ModeString);
        }
        else
        ;// print mode char error
    }
    else if (IsExistChannel(ChannelName))
    {
        ;// print not operator msg.
    }
    else
    ;// print channel does not exist msg.
}

void Server::Notice(const std::string &ChannelName, const std::string &TargetNick)
{
}
void Server::PrivMsg(const std::string &NickName, const std::string &Message)
{
    if (IsExistChannel(NickName))
    {
        //_channels[NickName].getOperator()->sendmessage(Message);
    }
    else if (IsExistClient(NickName, 0))
    {
        // findClient(NickName).sendmessage(Message);
    }
}
void Server::List()
{
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        std::cout << "channelname: " << it->first << "limit: " << it->second._clientLimit << ", operator: @" << it->second.getOperator()->_nick << ", topic: " << it->second._topic << "\n";
    }
}
void Server::List(const std::string &ChannelName)
{
    if (IsExistChannel(ChannelName))
    {
        std::cout << "channelname: " << ChannelName << "limit: " << _channels[ChannelName]._clientLimit << ", operator: @" << _channels[ChannelName].getOperator()->_nick << ", topic: " << _channels[ChannelName]._topic << "\n";
    }
    else
    ;// print channel not exist
}

bool Server::IsExistClient(const std::string &ClientName, const int val)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (!val)
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

bool Server::IsBannedClient(size_t idx, const std::string &ChannelName) //channela taşınabilir bu method
{
    std::vector<Client> &BannedVec = _channels[ChannelName].getBanned();
    for (std::vector<Client>::iterator it = BannedVec.begin(); it != BannedVec.end(); it++)
    {
        if (it->_nick == _clients[idx]._nick)
            return false;
    }
    return true;
}

bool Server::IsInChannel(size_t idx, const std::string &ChannelName) //channela taşınabilir bu method
{
    std::vector<Client> &MemberVec = _channels[ChannelName].getMembers();
    for (std::vector<Client>::iterator it = MemberVec.begin(); it != MemberVec.end(); it++)
    {
        if (it->_nick == _clients[idx]._nick)
            return false;
    }
    return true;
}

bool Server::HasChannelKey(const std::string &ChannelName)
{
    std::string NewKey = _channels[ChannelName].getKey();
    return !NewKey.empty();
}

bool Server::IsKeyWrong(const std::string &ChannelName, const std::string &Key)
{
    if (HasChannelKey(ChannelName))
    {
        if (_channels[ChannelName].getKey() == Key)
            return false;
        return true;
    }
    return true;
}

bool Server::IsChannelLimitFull(const std::string &ChannelName)
{
    return _channels[ChannelName].getMembers().size() == _channels[ChannelName]._clientLimit;
}

Client &Server::findClient(const std::string &NickName)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->_nick == NickName)
            return *it;
    }
}

void Client::ChangeMode(enum Mode &mode, const std::string &ModeString)
{
    if (ValidModeChars(ModeString))
    {
        std::map<char, enum Mode> modes = {std::make_pair('i', InviteOnly), std::make_pair('k', KeyChannel), std::make_pair('t', ProtectedTopic)};
        if (ModeString[0] == '+')
            mode = static_cast<enum Mode>((mode | modes[ModeString[1]]));
        else if (ModeString[0] == '-')
            mode = static_cast<enum Mode>((mode ^ modes[ModeString[1]]));
    }
    else
    ;// print not valid

}