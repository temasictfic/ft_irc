#include "../../inc/Server.hpp"

//A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). 
//These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.
//RPL_TOPIC (332)*
//RPL_NAMREPLY (353)*
//RPL_ENDOFNAMES (366)*

//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)
//ERR_TOOMANYCHANNELS (405)
//ERR_BADCHANNELKEY (475)*
//ERR_BANNEDFROMCHAN (474)*
//ERR_INVITEONLYCHAN (473)*
//ERR_CHANNELISFULL (471)*
//ERR_USERONCHANNEL (443)*
void Server::Join(Client &client,std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
      return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "JOIN", params, 1, 1) != 0)
        return;
    if (IsExistChannel(params[0]))
    {
        if (IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_USERONCHANNEL(client._nick, params[1], params[0]));
        else if (IsBannedClient(client, params[0]))
            sendServerToClient(client,ERR_BANNEDFROMCHAN(client._nick, params[0]));
        else if (IsChannelLimitFull(params[0]))
            sendServerToClient(client,ERR_CHANNELISFULL(client._nick, params[0]));
        else if (_channels.at(params[0])->_mode & InviteOnly && client._invitedchan != params[0])
            sendServerToClient(client,ERR_INVITEONLYCHAN(client._nick, params[0]));
        else if (params.size() < 2 && HasChannelKey(params[0]))
            sendServerToClient(client,ERR_BADCHANNELKEY(client._nick, params[0]));
        else if (params.size() == 2 && !PasswordMatched(_channels.at(params[0])->getKey(), params[1]))
            sendServerToClient(client,ERR_BADCHANNELKEY(client._nick, params[0]));
        else
        {
            client._invitedchan = "";
            _channels.at(params[0])->addMembers(client);
            sendServerToChannel(params[0], JOIN(client._nick, params[0])); //sendServerToCLient olabilir
            Topic(client, std::vector<std::string>(1,params[0]));
            Names(client, std::vector<std::string>(1,params[0]));
        }
    }
    else
    {
        if (InvalidLetter(params[0]) || params[0][0] != '#')
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("JOIN"), std::string("Forbidden letter in use as Channel name or didn't use #.")));
        else if (client._channel.size() == 4)
            sendServerToClient(client,ERR_TOOMANYCHANNELS(client._nick, params[0]));
        else
        {
            Channel* newish = new Channel(params[0],client);
            _channels.insert(std::make_pair(std::string(params[0]), newish));
            sendServerToClient(client, JOIN(client._nick, params[0]));
            newish->addMembers(client);
           if(params.size() == 2)
           {
                std::vector<std::string> vec;
                vec.push_back(params[0]);
                vec.push_back("+k");
                vec.push_back(params[1]);
                Mode(client,vec);
            }       
            sendServerToClient(client, MODE(std::string("ircserv"), params[0], "+o", client._nick));
            Topic(client, std::vector<std::string>(1,params[0]));
            Names(client, std::vector<std::string>(1,params[0]));
        }
    }
}
