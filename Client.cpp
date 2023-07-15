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
            //password assigned yazdır?;
            break;
        case RegistrationState::FullyRegistered:
            _password = Password;
            // password changed yazdır?;
            break;
        }
    }
    //Invalid Password hatası
}
void Client::Nick(const std::string &NickName)
{
    if(InvalidLetter(NickName) || InvalidPrefix(NickName) )
        //hata durumu;
        ;
    else if(IsExistClient(NickName,0))
        //hata durumu
        ;
    else if (_status == RegistrationState::UserRegistered)
    {
        _nick = LowercaseAlphabet(NickName);
        _status = RegistrationState::FullyRegistered;
    }
}

void Client::User(const std::string &Username)
{
    if(!IsExistClient(Username,1) && _status == RegistrationState::PassRegistered)
    {
        _username = LowercaseAlphabet(Username);
        _realname = "necati";
        _status = RegistrationState::NicknameRegistered;
    }
}
void Client::User(const std::string &Username, const std::string &Realname)
{
    if(!IsExistClient(Username,1) && _status == RegistrationState::PassRegistered)
    {   
        _username = LowercaseAlphabet(Username);
        _realname = LowercaseAlphabet(Realname);
        _status = RegistrationState::NicknameRegistered;
    }
    //protocole göre else yapıp hata mesajı yazdır.
}
bool Client::IsExistClient(const std::string &ClientName, const int val)
{
    for(std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if(!val)
        {
            if(it->_nick == ClientName)
                return true;
        }
        else
            if(it->_username == ClientName)
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
    if (IsExistChannel(ChannelName) )
    {
        if(IsInChannel(ChannelName, this))
        {
            //print client already in channel msg;
        }

        else if(isBannedClient(ChannelName))
        {
            //print client cannot join bcs in banned list of channel msg;
        }
        else
        {
            _channels[ChannelName].addMember(*this);
        }
    }
    else
    {
        if (InvalidChannelName(ChannelName))
        {
            //print invalid ChannelName error
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

}
void Client::Part(const std::string &ChannelName, const std::string &Reason)
{
    
}
void Client::Topic(const std::string &ChannelName, const std::string &TopicName)
{
    if(isExistChannel(ChannelName))
}
void Client::Names(const std::string &ChannelName)
{

}
void Client::Invite(const std::string &ChannelName,const std::string &NickName)
{

}
void Client::Mode(const std::string &ChannelName, const std::string &ModeString)
{

}
void Client::Notice(const std::string &ChannelName, const std::string &TargetNick)
{

}
void Client::PrivMsg(const std::string &NickName, const std::string &Message)
{

}
void Client::List()
{

}
void Client::List(const std::string &ChannelName)
{

}

bool InvalidPassword(const std::string &Password)
{
    if (Password.size() < 4 && Password.size() > 8)
    {
        return true;
        //Password size hatası?
    }
    for (size_t i = 0; i < Password.size() - 1; i++)
    {
        if (!isalpha(Password[i]))
        {
            //password invalid char hatası?
            return true;
        }
    }
    return false;
}

bool InvalidLetter(const std::string &Nick)
{
    std::string forbid = " .,*?!@";
    for (size_t i = 0; i < 4; i++)
    {
        if(Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
    
}
bool InvalidPrefix(const std::string &Nick)
{
    std::string prefixforbid = "$:#&/";
    for(int i = 0; i < 4 ; i++)
    {
        if(Nick[0] == prefixforbid[i] || isdigit(Nick[0]))
            return true;
    }
    return false;
}

bool Client::isBannedClient(const std::string &ChannelName)
{
    std::vector<Client> &BannedVec = _channels[ChannelName].getBanned();
    for(std::vector<Client>::iterator it = BannedVec.begin();  it != BannedVec.end(); it++)
    {
        if(it->_nick == _nick)
            return false;
    } 
    return true;
}