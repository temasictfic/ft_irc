#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Client::Client(int clientSocket): _nick(""), _username(""), _isOperator(false), _status(RegistrationState::None)
{
    _socket = clientSocket;
}
Client::~Client()
{
}

void Client::setPassword(const std::string& Password)
{
    _password = Password;
}

const int Client::getSocketFd() const
{
    return _socket;
}


