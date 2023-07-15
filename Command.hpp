#pragma once
#include <string>
class Command
{
    public:
     // socket descriptori alabilir bütün fonksiyonlar lazım olursa düzelt.
     // birden cok channel ismi alacaklar için vector e çevrilebilir.
        virtual void Pass(const std::string &Password) = 0;
        virtual void Nick(const std::string &NickName) = 0;
        virtual void User(const std::string &Username) = 0;
        virtual void User(const std::string &Username, const std::string &Realname) = 0;
        virtual void Ping() = 0;
        virtual void Quit(const std::string &Message) = 0;
        virtual void Join(const std::string &ChannelName) = 0;
        virtual void Join(const std::string &ChannelName, const std::string &ChannelKey) = 0;
        virtual void Part(const std::string &ChannelName, const std::string &Reason) = 0;
        virtual void Topic(const std::string &ChannelName, const std::string &TopicName) = 0;
        virtual void Names(const std::string &ChannelName) = 0;
        virtual void Invite(const std::string &ChannelName,const std::string &NickName) = 0;
        virtual void Mode(const std::string &ChannelName, const std::string &ModeString) = 0;
        virtual void Notice(const std::string &ChannelName, const std::string &TargetNick) = 0;
        virtual void PrivMsg(const std::string &NickName, const std::string &Message) = 0;
        virtual void List()= 0;
        virtual void List(const std::string &ChannelName) = 0;
};