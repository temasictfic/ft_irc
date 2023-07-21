#pragma once
#include <iostream>
#include "Server.hpp"
#include <vector>
#include <string>

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
    class Channel *_channel;
    enum RegistrationState _status;

    Client(int clientSocket, size_t idx);
    ~Client();

    void ChangeMode(enum Mode &mode, const std::string &ModeString);

    //getter setter
    const int getSocketFd() const;

};
