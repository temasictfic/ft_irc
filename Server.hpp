#pragma once
#include <iostream>
#include "Command.hpp"
#include <vector>
#include <map>
#include <string>


class Server : Command
{
protected:
    std::vector<class Client> _clients;
    std::map<std::string, class Channel> _channels;
public:
    Server(/* args */);
    ~Server();

    //Commands
    void Pass(size_t idx, const std::string &Password);
    void Nick(size_t idx, const std::string &NickName);
    void User(size_t idx, const std::string &Username);
    void User(size_t idx, std::string &Username, const std::string &Realname);
    void Ping(size_t idx);
    void Quit(size_t idx, const std::string &Message);
    void Join(size_t idx, const std::string &ChannelName);
    void Join(size_t idx, const std::string &ChannelName, const std::string &ChannelKey);
    void Part(size_t idx, const std::string &ChannelName, const std::string &Reason);
    void Topic(size_t idx, const std::string &ChannelName, const std::string &TopicName);
    void Names(const std::string &ChannelName);
    void Invite(size_t idx, const std::string &ChannelName,const std::string &NickName);
    void Mode(size_t idx, const std::string &ChannelName, const std::string &ModeString);
    void Notice(const std::string &ChannelName, const std::string &TargetNick);
    void PrivMsg(const std::string &NickName, const std::string &Message);
    void List();
    void List(const std::string &ChannelName);


    //Controls
    bool IsExistClient(const std::string &ClientName, const int val);
    bool IsExistChannel(const std::string &ChannelName);
    bool IsBannedClient(size_t idx, const std::string &ChannelName);
    bool IsInChannel(size_t idx, const std::string &ChannelName);
    bool HasChannelKey(const std::string &ChannelName);
    bool IsKeyWrong(const std::string &ChannelName, const std::string &Key);
    bool IsChannelLimitFull(const std::string &ChannelName);
    Client &findClient(const std::string &NickName);

    void ChangeMode(enum Mode &mode, const std::string &ModeString);

    bool isValidIp();
    bool isValidPort();
    void run();
    
};

