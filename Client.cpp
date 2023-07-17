#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Client::Client(/* args */)
{
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

}


