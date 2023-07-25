#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Replies.hpp"
#include "../inc/Utils.hpp"
// isbannedclient true false yerdeğiştirdim

// enum Mode : unsigned int;
const int MAX_CLIENTS = 10; // Maximum number of clients to handle
const int BUFFER_SIZE = 1024;

class Server
{
private:
    sockaddr_in  _serverAddress;
    int _serverSocketFd;
    int _port;
    std::string _password;
    std::vector<class Client*> _clients;
    std::map<std::string, class Channel*> _channels;
    
public:
    std::map<std::string, void (Server::*)(class Client &, std::vector<std::string>)> cmds;

    Server(const std::string &Port, const std::string &Password);
    ~Server();

    // Server.cpp
    Server Listen();
    void Run();
    void Serve(fd_set readSet);
    void ProcessCommand(std::string &message, Client *client);
    void ProcessChat(const std::string &message, Client *client);

    // Commands
    void Pass(class Client &, std::vector<std::string>);
    void Nick(class Client &, std::vector<std::string>);
    void User(class Client &, std::vector<std::string>);
    void Ping(class Client &, std::vector<std::string>);
    void Quit(class Client &, std::vector<std::string>);
    void Join(class Client &, std::vector<std::string>);
    void Part(class Client &, std::vector<std::string>);
    void Topic(class Client &, std::vector<std::string>);
    void Names(class Client &, std::vector< std::string>);
    void Invite(class Client &, std::vector< std::string>);
    void Mode(class Client &, std::vector<std::string>);
    void Kick(class Client &, std::vector<std::string>);
    void Notice(class Client &, std::vector< std::string>);
    void PrivMsg(class Client &, std::vector< std::string>);
    void List(class Client &, std::vector<std::string>);

    // ServerUtils.cpp
    bool IsExistClient(const std::string &ClientName, const int flag);
    bool IsExistChannel(const std::string &ChannelName);
    bool IsBannedClient(class Client &, const std::string &ChannelName);
    bool IsInChannel(class Client &, const std::string &ChannelName);
    // bool IsOperator(Client &client, const std::string& Nick);
    bool HasChannelKey(const std::string &ChannelName);
    bool IsKeyWrong(const std::string &ChannelName, const std::string &Key);
    bool IsChannelLimitFull(const std::string &ChannelName);
    Client &findClient(const std::string &NickName);
    int ParamsSizeControl(std::vector<std::string> params, size_t index, size_t optional);
    bool PasswordMatched(const std::string &PasswordOrigin, const std::string &PasswordGiven);

    // Send messagges
    int sendServerToClient(Client &, const std::string &message);
    int sendServerToChannel(const std::string &ChannelName, const std::string &message);
    int sendClientToClient(Client &sender, Client &reciever, const std::string &message);
    int sendClientToChannel(Client &sender, const std::string &ChannelName, const std::string &message);

    const std::string &getPassword() const;

    class InvalidPortException : public std::exception
    {
        virtual const char *what() const throw()
        {
            return "Wrong Port\n";
        }
    };
};

#endif