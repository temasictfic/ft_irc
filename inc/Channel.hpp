#pragma once
#include <iostream>
#include <vector>
#include <map>

enum Mode
{
    Default = 1,
    InviteOnly = 2,
    KeyChannel = 4,
    ProtectedTopic = 8
};

class Channel
{
private:
    class Client *_operator;
    std::string _key;
    std::vector<class Client*> _banned;
    std::vector<class Client*> _members;
public:
    int _mode;
    std::string _name;
    std::string _topic;
    unsigned int _clientLimit;
    

    Channel();
    Channel(std::string ChannelName, class Client &);
    ~Channel();

    const std::string &getKey() const;
    void setKey(const std::string &key);

    std::vector<class Client*>& getBanned();
    std::vector<class Client*>& getMembers();

    class Client  *getOperator() const;
    void setOperator(class Client *client);

    void addMembers(class Client &client);
    void removeMembers(class Client &client);

    void addBanned(class Client &client);
    void removeBanned(class Client &client);


    //gerekirse vectorlere add ve remove uye fonksiyonlari yaz

};

bool ChangeMode(Client &client, const std::vector<std::string> &ModeString, std::map<char, int> modes);

