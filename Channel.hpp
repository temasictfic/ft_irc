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

    std::string getTopic() const;
    void setTopic(const std::string &topic);
    
    std::string getKey() const;
    void setKey(const std::string &key);
    
    unsigned int getClientLimit() const;
    void setClientLimit(const unsigned int &clientLimit);

    //gerekirse vectorlere add ve remove uye fonksiyonlari yaz


    Channel(/* args */);
    ~Channel();
};


