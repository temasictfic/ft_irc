#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

class Server
{
protected:
    std::vector<class Client> _clients;
    std::map<std::string, class Channel> _channels;
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
