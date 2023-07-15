#pragma once
#include <iostream>
#include <vector>
class Server
{
protected:
    std::vector<class Client> _clients;
    std::vector<class Channel> _channels;
public:
    Server(/* args */);
    ~Server();
    bool isValidIp();
    bool isValidPort();
    void run();
    
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}
