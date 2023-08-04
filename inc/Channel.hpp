#pragma once
#include <iostream>
#include <vector>
#include <map>

enum Mode
{
    ProtectedTopic = 1,
    InviteOnly = 2,
    KeyChannel = 4,
    ChannelLimit = 8
};

class Channel
{
private:
    class Client *_operator;
    std::string _key;
    std::vector<class Client*> _banned;
    std::vector<class Client*> _members;
public:
    std::string _name;
    std::string _topic;
    int _mode;
    unsigned int _clientLimit;
    

    Channel(std::string ChannelName, class Client &);
    ~Channel();

    const std::string &getKey() const;
    void setKey(const std::string &key);

    std::vector<class Client*>& getBanned();
    std::vector<class Client*>& getMembers();

    class Client  *getOperator() const;
    void setOperator(class Client *client);

    void addMember(class Client &client);
    void removeMember(class Client &client);

    void addBanned(class Client &client);
    void removeBanned(class Client &client);

    bool ChangeBannedMode(Client &banned, const std::string &ModeString, bool isbanned);
    bool ChangeModeTwoParams(const std::string& ModeString, const std::map<char,int>& modes);
    bool ChangeModeThreeParams(const std::string& ModeString, const std::string& ModeArg, const std::map<char,int>& modes);

};
