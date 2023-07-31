#include "../inc/Server.hpp"

Client::Client(int clientSocket) : _hostname("unknown"), _nick(""), _username(""), _realname(""), _invitedchan(""), _status(None) , _online(true), _channel()
{
    _socket = clientSocket;
    //_fileinfos = std::vector<std::string>();
}

/* Client::Client()
{
    
} */

Client::~Client()
{
    //_channel = NULL;
    
}

int Client::getSocketFd() const
{
    return _socket;
}

void Client::addHostname(sockaddr_in& serverAddress)
{
    char hostname[1024];
    if (getnameinfo((struct sockaddr *) &serverAddress, sizeof(serverAddress), hostname, 1024, NULL, 0, NI_NUMERICSERV) != 0)
    {
        std::cerr << "Error: failed to get client hostname!\n";
        return;
    }
    _hostname = std::string(hostname);
}