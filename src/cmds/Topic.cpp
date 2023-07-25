#include "../../inc/Server.hpp"
#include "../../inc/Utils.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Replies.hpp"

//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)*
//ERR_NOTONCHANNEL (442)*
//ERR_CHANOPRIVSNEEDED (482)*
//RPL_NOTOPIC (331)
//RPL_TOPIC (332)

void Server::Topic(Client &client, std::vector<std::string > params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    if(params.empty())
    {
        sendServerToClient(client,ERR_NEEDMOREPARAMS(std::string("/TOPIC")));
        return;
    }
    std::string message;
    for(size_t index = 1; index < params.size(); index++)
        message += params[index];
    if(params.size() < 3)
        message = "";
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if ( _channels.at(params[0])->_mode == ProtectedTopic &&  _channels.at(params[0])->getOperator()->_nick == client._nick)
        {
             _channels.at(params[0])->_topic = message;
            sendServerToChannel(params[0], std::string("Topic: " + message));
        }
        else if ( _channels.at(params[0])->_mode == ProtectedTopic)
               sendServerToClient(client, ERR_CHANOPRIVSNEEDED(params[0]));
        else
        {
             _channels.at(params[0])->_topic = message;
            sendServerToChannel(params[0], std::string("Topic: " + message));
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(params[0]));
    }
}
