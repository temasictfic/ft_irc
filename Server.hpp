#pragma once
#include <iostream>
#include "Command.hpp"
#include <vector>
#include <map>
#include <string>

const int MAX_CLIENTS = 10; // Maximum number of clients to handle
const int BUFFER_SIZE = 1024;


class Server
{
private:
    int _serverSocketFd;
    int _port;
    std::string _password;
    std::vector<class Client> _clients;
    std::map<std::string, class Channel> _channels;
    std::map<std::string, void(Server::*)(Client &, std::vector<std::string>)> _cmds;
    //struct sockaddr_in  _serverAddress;
public:
    Server(const std::string &Port, const std::string &Password);
    bool ReadySocketandPort();
    void Run();
    void processCommand(const std::string &command, std::pair<const std::string &,const std::string &> params, Client &client)
    ~Server();

    //Commands
    void Pass(class Client &, std::vector<std::string>);
    void Nick(class Client &, std::vector<std::string>);
    void User(class Client &, std::vector<std::string>);
    void Ping(class Client &, std::vector<std::string>);
    void Quit(class Client &,   std::vector<std::string>);
    void Join(class Client &, std::vector<std::string>)
    void Part(class Client &, std::vector<std::string>);
    void Topic(class Client &,std::vector<std::string>);
    void Names(class Client &,std::vector<std::string>);
    void Invite(class Client &, std::vector<std::string>);
    void Mode(class Client &, std::vector<std::string>);
    void Notice(class Client &, std::vector<std::string>);
    void PrivMsg(class Client &, std::vector<std::string>);
    void List(class Client &, std::vector<std::string>);


    //Controls
    bool IsExistClient(const std::string &ClientName, const int val);
    bool IsExistChannel(const std::string &ChannelName);
    bool IsBannedClient(class Client &, const std::string &ChannelName);
    bool IsInChannel(class Client &, const std::string &ChannelName);
    bool HasChannelKey(const std::string &ChannelName);
    bool IsKeyWrong(const std::string &ChannelName, const std::string &Key);
    bool IsChannelLimitFull(const std::string &ChannelName);
    Client &findClient(const std::string &NickName);

    void ChangeMode(enum Mode &mode, const std::string &ModeString);

    bool isValidIp();
    bool isValidPort();
    void run();
    
    class InvalidPortException : public std::exception
    {
        virtual const char *what() const throw()
        {
            std::cerr << "Wrong Port\n";
        }
    };
};

