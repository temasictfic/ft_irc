#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Client::Client(int clientSocket, size_t idx) : _nick(""), _username(""), _status(RegistrationState::None) , _channel(nullptr)
{
    _socket = clientSocket;
    _idx = idx;
}
Client::~Client()
{
    
}

const int Client::getSocketFd() const
{
    return _socket;
}


