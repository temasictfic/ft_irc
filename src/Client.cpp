#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Utils.hpp"

Client::Client(int clientSocket, size_t idx) : _nick(""), _username(""), _realname(""), _status(RegistrationState::None) , _channel(NULL)
{
    _socket = clientSocket;
    _idx = idx;
}
Client::~Client()
{
    
}

int Client::getSocketFd() const
{
    return _socket;
}


