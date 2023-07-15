#pragma once
#include <iostream>
#include <vector>
enum Mode  
{
    InviteOnly,
    KeyChannel,
    Secret,
    ProtectedTopic,
    NoexternalMessage,
};

class Channel
{
private:
    class Client * _operator;
    std::string _topic;
    std::string _key;
    unsigned int _clientLimit;
    std::vector<class Client> _banned;
    std::vector<class Client> _members;
    enum Mode _mode;
public:
    Channel();
    Channel(std::string ChannelName);
    ~Channel();

    std::string getTopic() const;
    void setTopic(const std::string &topic);
    
    std::string getKey() const;
    void setKey(const std::string &key);

    std::vector<class Client> &getBanned();
    std::vector<class Client> &getMembers();

    size_t getClientLimit() const;
    void setClientLimit(const unsigned int &clientLimit);

    void addMembers(class Client &Obj);
    void removeMembers(class Client &Obj);

    void addBanned(class Client &Obj);
    void removeBanned(class Client &Obj);
    //gerekirse vectorlere add ve remove uye fonksiyonlari yaz


};


