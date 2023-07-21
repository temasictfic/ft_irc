#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>

//isbannedclient true false yerdeğiştirdim


const int MAX_CLIENTS = 10; // Maximum number of clients to handle
const int BUFFER_SIZE = 1024;

const std::map<std::string, void (Server::*)(class Client &, std::vector<const std::string &>)> cmds =
    {{"PASS", &Server::Pass},
     {"NICK", &Server::Nick},
     {"USER", &Server::User},
     {"PING", &Server::Ping},
     {"QUIT", &Server::Quit},
     {"JOIN", &Server::Join},
     {"PART", &Server::Part},
     {"TOPIC", &Server::Topic},
     {"NAMES", &Server::Names},
     {"INVITE", &Server::Invite},
     {"MODE", &Server::Mode},
     {"KICK", &Server::Kick},
     {"NOTICE", &Server::Notice},
     {"PRIVMSG", &Server::PrivMsg},
     {"LIST", &Server::List}};

class Server
{
private:
    int _serverSocketFd;
    int _port;
    std::string _password;
    std::vector<class Client> _clients;
    std::map<std::string, class Channel> _channels;

    // struct sockaddr_in  _serverAddress;
public:
    Server(const std::string &Port, const std::string &Password);
    ~Server();

    //Server.cpp
    bool ReadySocketandPort();
    void Run();
    void Detect(fd_set readSet);
    void ParseAndExecute(const std::string &message, Client &client);

    // Commands
    void Pass(class Client &, std::vector<const std::string &>);
    void Nick(class Client &, std::vector<const std::string &>);
    void User(class Client &, std::vector<const std::string &>);
    void Ping(class Client &, std::vector<const std::string &>);
    void Quit(class Client &, std::vector<const std::string &>);
    void Join(class Client &, std::vector<const std::string &>);
    void Part(class Client &, std::vector<const std::string &>);
    void Topic(class Client &, std::vector<const std::string &>);
    void Names(class Client &, std::vector<const std::string &>);
    void Invite(class Client &, std::vector<const std::string &>);
    void Mode(class Client &, std::vector<const std::string &>);
    void Kick(class Client &, std::vector<const std::string &>);
    void Notice(class Client &, std::vector<const std::string &>);
    void PrivMsg(class Client &, std::vector<const std::string &>);
    void List(class Client &, std::vector<const std::string &>);

    // ServerUtils.cpp
    bool IsExistClient(const std::string &ClientName, const int flag);
    bool IsExistChannel(const std::string &ChannelName);
    bool IsBannedClient(class Client &, const std::string &ChannelName);
    bool IsInChannel(class Client &, const std::string &ChannelName);
    //bool IsOperator(Client &client, const std::string& Nick);
    bool HasChannelKey(const std::string &ChannelName);
    bool IsKeyWrong(const std::string &ChannelName, const std::string &Key);
    bool ChangeMode(enum Mode &mode, const std::string &ModeString);
    bool IsChannelLimitFull(const std::string &ChannelName);
    Client &findClient(const std::string &NickName);
    int ParamsSizeControl(std::vector<const std::string&> params, size_t index, size_t optional);
    bool PasswordMatched(const std::string& PasswordOrigin, const std::string& PasswordGiven);


    //Server.cpp
    int sendServerToClient(Client&, const std::string &message);
    int sendServerToChannel(const std::string& ChannelName, const std::string& message);
    int sendClientToClient(Client& sender, Client& reciever, const std::string &message);
    int sendClientToChannel(Client& sender, const std::string& ChannelName, const std::string &message);

    const std::string& getPassword() const;

    class InvalidPortException : public std::exception
    {
        virtual const char *what() const throw()
        {
            std::cerr << "Wrong Port\n";
        }
    };
};
