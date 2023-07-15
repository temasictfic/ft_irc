#include "Client.hpp"
Client::Client(/* args */)
{

}
Client::~Client()
{

}
void Client::Pass(const std::string &Password)
{
    if(_status == RegistrationState::None)
    {
        _password = Password;
        _status = RegistrationState::PassRegistered;
    }
}
void Client::Nick(const std::string &NickName)
{
    if(NickLetterControl(NickName))
        //hata durumu;
        ;
    else if(NickPrefixControl )
       //hata durumu
        ;
    else if(IsExistClient(NickName,0))
        //hata durumu
        ;
    else if (_status == RegistrationState::UserRegistered)
    {
        _nick = NickName;
        _status = RegistrationState::FullyRegistered;
    }
}

void Client::User(const std::string &Username)
{
    if(!IsExistClient(Username,1) && _status == RegistrationState::PassRegistered)
    {
        _username = Username;
        _realname = "Necati";
        _status = RegistrationState::NicknameRegistered;
    }
}
void Client::User(const std::string &Username, const std::string &Realname)
{
    if(!IsExistClient(Username,1) && _status == RegistrationState::PassRegistered)
    {   
        _username = Username;
        _realname = Realname;
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
     for(std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if(it->_nick == ChannelName)
            return true;
    }
    return false;
}
void Client::Ping()
{

}
void Client::Quit(const std::string &Message)
{

}
void Client::Join(const std::string &ChannelName)
{

}
void Client::Join(const std::string &ChannelName, const std::string &ChannelKey)
{

}
void Client::Part(const std::string &ChannelName, const std::string &Reason)
{

}
void Client::Topic(const std::string &ChannelName, const std::string &TopicName)
{

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

bool NickLetterControl(const std::string &Nick)
{
    std::string forbid = ",*?!";
    for (size_t i = 0; i < 4; i++)
    {
        if(Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
    
}
bool NickPrefixControl(const std::string &Nick)
{
    std::string prefixforbid = "$#&/";
    for(int i = 0; i < 4 ; i++)
    {
        if(Nick[0] == prefixforbid[i])
            return true;
    }
    return false;
}