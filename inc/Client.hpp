#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Channel.hpp"
#include "Server.hpp"

enum RegistrationState {
    None,
    PassRegistered,
    NickRegistered,
    UsernameRegistered,
};

class Client
{
private:
    int _socket;
public:
    std::string _hostname;
    std::string _nick;
    std::string _username;
    std::string _realname;
    std::string _invitedchan;
    //std::vector<std::string> _fileinfos;
    enum RegistrationState _status;
    bool _online;
    std::map<std::string, Channel*> _channel;

    Client(int clientSocket);
    //Client();
    ~Client();

    //getter setter
    int getSocketFd() const;

    void addHostname(sockaddr_in& serverAddress);

};
