#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Channel.hpp"

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
    size_t _idx;
    std::string _nick;
    std::string _username;
    std::string _realname;
    enum RegistrationState _status;
    class Channel *_channel;

    Client(int clientSocket, size_t idx);
    ~Client();

    //getter setter
    int getSocketFd() const;

};
