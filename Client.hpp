#pragma once
#include <iostream>
#include "Command.hpp"
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
    std::string _password;

public:
    std::string _nick;
    std::string _username;
    std::string _realname;
    bool _isOperator;
    enum RegistrationState _status;

    Client(int clientSocket);
    ~Client();

    void ChangeMode(enum Mode &mode, const std::string &ModeString);

    //getter setter
    void setPassword(const std::string& Password);
    const int getSocketFd() const;

};
