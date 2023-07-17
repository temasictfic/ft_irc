#pragma once
#include <iostream>
#include "Command.hpp"
#include <vector>
#include <map>
#include <string>

const int MAX_CLIENTS = 10; // Maximum number of clients to handle
const int BUFFER_SIZE = 1024;

std::map<std::string, int> cmds = { std::make_pair("PASS", 0),
                                    std::make_pair("NICK", 1),
                                    std::make_pair("USER", 2),
                                    std::make_pair("PING", 3),
                                    std::make_pair("QUIT", 4),
                                    std::make_pair("JOIN", 5),
                                    std::make_pair("PART", 6),
                                    std::make_pair("TOPIC", 7),
                                    std::make_pair("NAMES", 8),
                                    std::make_pair("INVITE", 9),
                                    std::make_pair("MODE", 10),
                                    std::make_pair("NOTICE", 11),
                                    std::make_pair("PRIVMSG", 12),
                                    std::make_pair("LIST", 13)};

class Server : Command
{
private:
    int _serverSocketFd;
    int _port;
    std::string _password;
    std::vector<class Client> _clients;
    std::map<std::string, class Channel> _channels;
    //struct sockaddr_in  _serverAddress;
public:
    Server(const std::string &Port, const std::string &Password);
    bool ReadySocketandPort();
    void Run();
    void processCommand(const std::string &command, std::pair<const std::string &,const std::string &> params, Client &client)
    ~Server();

    //Commands
    void Pass(class Client &, const std::string &Password);
    void Nick(class Client &, const std::string &NickName);
    void User(class Client &, const std::string &Username);
    void User(class Client &, const std::string &Username, const std::string &Realname);
    void Ping(class Client &);
    void Quit(class Client &, const std::string &Message);
    void Join(class Client &, const std::string &ChannelName);
    void Join(class Client &, const std::string &ChannelName, const std::string &ChannelKey);
    void Part(class Client &, const std::string &ChannelName, const std::string &Reason);
    void Topic(class Client &, const std::string &ChannelName, const std::string &TopicName);
    void Names(const std::string &ChannelName);
    void Invite(class Client &, const std::string &ChannelName,const std::string &NickName);
    void Mode(class Client &, const std::string &ChannelName, const std::string &ModeString);
    void Notice(const std::string &ChannelName, const std::string &TargetNick);
    void PrivMsg(const std::string &NickName, const std::string &Message);
    void List();
    void List(const std::string &ChannelName);


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

