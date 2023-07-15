#pragma once
#include <iostream>
#include "Command.hpp"
#include "Server.hpp"
#include <vector>
#include <string>

enum RegistrationState {
    None,
    PassRegistered,
    UserRegistered,
    NicknameRegistered,
    FullyRegistered
};

class Client : Server, Command
{
private:
    std::string _password;
    std::string _nick;
    std::string _username;
    std::string _realname;
    bool _isOperator;
    enum RegistrationState _status;
public:
    Client(/* args */);
    ~Client();

    //Commands
    void Pass(const std::string &Password);
    void Nick(const std::string &NickName);
    void User();
    void User(const std::string &Username);
    void User(const std::string &Username, const std::string &Realname);
    void Ping();
    void Quit(const std::string &Message);
    void Join(const std::string &ChannelName);
    void Join(const std::string &ChannelName, const std::string &ChannelKey);
    void Part(const std::string &ChannelName, const std::string &Reason);
    void Topic(const std::string &ChannelName, const std::string &TopicName);
    void Names(const std::string &ChannelName);
    void Invite(const std::string &ChannelName,const std::string &NickName);
    void Mode(const std::string &ChannelName, const std::string &ModeString);
    void Notice(const std::string &ChannelName, const std::string &TargetNick);
    void PrivMsg(const std::string &NickName, const std::string &Message);
    void List();
    void List(const std::string &ChannelName);

    //Controls
    bool IsExistClient(const std::string &ClientName, const int val);
    bool IsExistChannel(const std::string &ChannelName);
    bool InvalidPassword(const std::string &Password);
    bool InvalidLetter(const std::string &Nick);
    bool InvalidPrefix(const std::string &Nick);
    bool IsBannedClient(const std::string &ChannelName);
    bool IsInChannel(const std::string &ChannelName);
    bool HasChannelKey(const std::string &ChannelName);
    bool IsKeyWrong(const std::string &ChannelName, const std::string &Key);
    bool IsChannelLimitFull(const std::string &ChannelName);
};