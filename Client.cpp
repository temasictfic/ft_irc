#include "Client.hpp"
#include "Channel.hpp"
#include "utilits.hpp"
Client::Client(/* args */)
{
}
Client::~Client()
{
}
void Client::Pass(const std::string &Password)
{
    if (!InvalidPassword(Password))
    {
        switch (_status)
        {
        case RegistrationState::None:
            _password = Password;
            _status = RegistrationState::PassRegistered;
            // password assigned yazdır?;
            break;
        case RegistrationState::FullyRegistered:
            _password = Password;
            // password changed yazdır?;
            break;
        }
    }
    // Invalid Password hatası
}
void Client::Nick(const std::string &NickName)
{
    if (InvalidLetter(NickName) || InvalidPrefix(NickName))
        // hata durumu;
        ;
    else if (IsExistClient(NickName, 0))
        // hata durumu
        ;
    else if (_status == RegistrationState::UserRegistered)
    {
        _nick = LowercaseAlphabet(NickName);
        _status = RegistrationState::FullyRegistered;
    }
}

void Client::User(const std::string &Username)
{
    if (!IsExistClient(Username, 1) && _status == RegistrationState::PassRegistered)
    {
        _username = LowercaseAlphabet(Username);
        _realname = "necati";
        _status = RegistrationState::NicknameRegistered;
    }
}
void Client::User(const std::string &Username, const std::string &Realname)
{
    if (!IsExistClient(Username, 1) && _status == RegistrationState::PassRegistered)
    {
        _username = LowercaseAlphabet(Username);
        _realname = LowercaseAlphabet(Realname);
        _status = RegistrationState::NicknameRegistered;
    }
    // protocole göre else yapıp hata mesajı yazdır.
}
bool Client::IsExistClient(const std::string &ClientName, const int val)
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
bool Client::IsExistChannel(const std::string &ChannelName)
{
    std::map<std::string, class Channel>::iterator it = _channels.find(ChannelName);
    return (it != _channels.end()) ? true : false;
}

void Client::Ping()
{
    sendMessageToServer("Ping");
}
void Client::Quit(const std::string &Message)
{
    // print client nickname quited msg
    close(_socket);
}
void Client::Join(const std::string &ChannelName)
{
    if (IsExistChannel(ChannelName))
    {
        if (IsInChannel(ChannelName))
        {
            // print client already in channel msg;
        }

        else if (IsBannedClient(ChannelName))
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
            _channels[ChannelName].addMembers(*this);
        }
    }
    else
    {
        if (IsValidChannelName(ChannelName))
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
void Client::Join(const std::string &ChannelName, const std::string &ChannelKey)
{
    if (IsExistChannel(ChannelName))
    {
        if (IsInChannel(ChannelName))
        {
            // print client already in channel msg;
        }

        else if (IsBannedClient(ChannelName))
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
            _channels[ChannelName].addMembers(*this);
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
void Client::Part(const std::string &ChannelName, const std::string &Reason)
{
    // print reason
    _channels[ChannelName].removeMembers(*this);
}
void Client::Topic(const std::string &ChannelName, const std::string &TopicName)
{
    if (InvalidPrefix(TopicName) || InvalidLetter(TopicName))
        // print error doesnt allow this letter
        if (IsExistChannel(ChannelName) && IsInChannel(ChannelName))
        {
            if (_channels[ChannelName]._mode == Mode::ProtectedTopic && _channels[ChannelName].getOperator()->_nick == _nick)
                _channels[ChannelName].setTopic(TopicName);
            else if (_channels[ChannelName]._mode == Mode::ProtectedTopic)
            {
                // print error
            }
            else
                _channels[ChannelName].setTopic(TopicName);
        }
}
void Client::Names(const std::string &ChannelName)
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
void Client::Invite(const std::string &ChannelName, const std::string &NickName)
{
    if (!IsExistClient(NickName, 0))
        // print error
        // check there is a man with this nick in clients
        if (IsExistChannel(ChannelName) && IsInChannel(ChannelName))
        {
            if (_channels[ChannelName]._mode == Mode::InviteOnly && _channels[ChannelName].getOperator()->_nick == _nick)
                _channels[ChannelName].addMembers(findClientNick(NickName));
            else if (_channels[ChannelName]._mode == Mode::InviteOnly)
            {
                // print error
            }
            else
                _channels[ChannelName].addMembers(findClientNick(NickName));
        }
}
void Client::Mode(const std::string &ChannelName, const std::string &ModeString)
{
    if (IsExistChannel(ChannelName) && _channels[ChannelName].getOperator()->_nick == _nick)
    {
        if (ModeString.size() == 2)
        {
            ChangeMode(_channels[ChannelName]._mode, ModeString);
        }
        else
        // print mode char error
    }
    else if (IsExistChannel(ChannelName))
    {
        // print not operator msg.
    }
    else
    // print channel does not exist msg.
}

void Client::Notice(const std::string &ChannelName, const std::string &TargetNick)
{
}
void Client::PrivMsg(const std::string &NickName, const std::string &Message)
{
    if (IsExistChannel(NickName))
    {
        //_channels[NickName].getOperator()->sendmessage(Message);
    }
    else if (IsExistClient(NickName, 0))
    {
        // findClientNick(NickName).sendmessage(Message);
    }
}
void Client::List()
{
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        std::cout << "channelname: " << it->first << "limit: " << it->second._clientLimit << ", operator: @" << it->second.getOperator()->getNick() << ", topic: " << it->second.getTopic() << "\n";
    }
}
void Client::List(const std::string &ChannelName)
{
    if (IsExistChannel(ChannelName))
    {
        std::cout << "channelname: " << ChannelName << "limit: " << _channels[ChannelName]._clientLimit << ", operator: @" << _channels[ChannelName].getOperator()->getNick() << ", topic: " << _channels[ChannelName].getTopic() << "\n";
    }
    else
    // print channel not exist
}

bool Client::InvalidPassword(const std::string &Password)
{
    if (Password.size() < 4 && Password.size() > 8)
    {
        return true;
        // Password size hatası?
    }
    for (size_t i = 0; i < Password.size() - 1; i++)
    {
        if (!isalpha(Password[i]))
        {
            // password invalid char hatası?
            return true;
        }
    }
    return false;
}

bool Client::InvalidLetter(const std::string &Nick)
{
    std::string forbid = " .,*?!@";
    for (size_t i = 0; i < 4; i++)
    {
        if (Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
}
bool Client::InvalidPrefix(const std::string &Nick)
{
    std::string prefixforbid = "$:#&/";
    for (int i = 0; i < 4; i++)
    {
        if (Nick[0] == prefixforbid[i] || isdigit(Nick[0]))
            return true;
    }
    return false;
}

bool Client::IsBannedClient(const std::string &ChannelName)
{
    std::vector<Client> &BannedVec = _channels[ChannelName].getBanned();
    for (std::vector<Client>::iterator it = BannedVec.begin(); it != BannedVec.end(); it++)
    {
        if (it->_nick == _nick)
            return false;
    }
    return true;
}

bool Client::IsInChannel(const std::string &ChannelName)
{
    std::vector<Client> &MemberVec = _channels[ChannelName].getMembers();
    for (std::vector<Client>::iterator it = MemberVec.begin(); it != MemberVec.end(); it++)
    {
        if (it->_nick == _nick)
            return false;
    }
    return true;
}

bool Client::HasChannelKey(const std::string &ChannelName)
{
    std::string NewKey = _channels[ChannelName].getKey();

    if (NewKey[0])
        return true;
    return false;
}

bool Client::IsKeyWrong(const std::string &ChannelName, const std::string &Key)
{
    if (HasChannelKey(ChannelName))
    {
        if (_channels[ChannelName].getKey() == Key)
            return false;
        return true;
    }
    return true;
}

bool Client::IsChannelLimitFull(const std::string &ChannelName)
{
    size_t i = 0;
    std::vector<Client> &ClientVec = _channels[ChannelName].getMembers();
    for (std::vector<Client>::iterator it = ClientVec.begin(); it != ClientVec.end(); it++)
    {
        i++;
    }
    if (_channels[ChannelName].getClientLimit() > i)
        return false;
    return true;
}

const std::string &Client::getNick() const
{
    return _nick;
}

const std::vector<Client> &Client::getClients() const
{
    return _clients;
}

Client &Client::findClientNick(const std::string &NickName)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); i++)
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
    // print not valid

}