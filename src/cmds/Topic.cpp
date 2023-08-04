#include "../../inc/Server.hpp"


//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)*
//ERR_NOTONCHANNEL (442)*
//ERR_CHANOPRIVSNEEDED (482)*
//RPL_NOTOPIC (331)*
//RPL_TOPIC (332)*

void Server::Topic(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "TOPIC", params, 1, 10) != 0)  
        return;
    size_t count = params.size();
    std::string message;
    if(count > 1)
    {
        message = (params[1][0] == ':') ? params[1].substr(1) : params[1];
        for (size_t i = 2; i < count; i++)
            message += " " + params[i];
    }
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]) && !IsBannedClient(client,params[0]))
    {
        if(count == 1)
        {
            if (_channels.at(params[0])->_topic == "")
                sendServerToClient(client, RPL_NOTOPIC(client._nick, params[0]));
            else
                sendServerToClient(client, RPL_TOPIC(client._nick, params[0], _channels.at(params[0])->_topic));
        }
        else if ((_channels.at(params[0])->_mode & ProtectedTopic)  &&  IsOperator(client, params[0]))
        {
             _channels.at(params[0])->_topic = message;
            sendServerToChannel(params[0], RPL_TOPIC(client._nick,params[0],message));
        }
        else if ((_channels.at(params[0])->_mode & ProtectedTopic))
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, params[0]));
        else
        {
             _channels.at(params[0])->_topic = message;
            sendServerToChannel(params[0], RPL_TOPIC(client._nick,params[0],message));
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(client._nick, params[0]));
    }
}
