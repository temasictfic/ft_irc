#pragma once
#include <iostream>
#include <vector>
#include <map>

enum Mode
{
    Default = 0,
    InviteOnly = 1,
    KeyChannel = 2,
    ProtectedTopic = 4
};

const std::map<char, enum Mode> modes = {{'i', InviteOnly}, {'k', KeyChannel}, {'t', ProtectedTopic}};

class Channel
{
private:
    class Client *_operator;
    std::string _key;
    std::vector<class Client> _banned;
    std::vector<class Client> _members;
public:
    enum Mode _mode;
    std::string _name;
    std::string _topic;
    unsigned int _clientLimit;
    

    Channel(const std::string& ChannelName, Client& op);
    ~Channel();

    const std::string &getKey() const;
    void setKey(const std::string &key);

    std::vector<class Client> &getBanned();
    std::vector<class Client> &getMembers();

    class Client  *getOperator() const;
    void setOperator(class Client *Obj);

    void addMembers(class Client &Obj);
    void removeMembers(class Client &Obj);

    void addBanned(class Client &Obj);
    void removeBanned(class Client &Obj);
    //gerekirse vectorlere add ve remove uye fonksiyonlari yaz


};


