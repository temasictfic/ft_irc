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
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    int err = ParamsSizeControl(params, 1, 10);
    if ( err != 0)  
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("TOPIC")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("TOPIC"), std::string("Excessive argument is given")));
        return;
    }
    size_t count = params.size();
    std::string message;
    if(count > 1)
    {
        message = params[1].substr(1);
        for (size_t i = 0; i < count - 2; i++)
            message += " " + params[i];
    }
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if ( _channels.at(params[0])->_mode == ProtectedTopic &&  _channels.at(params[0])->getOperator()->_nick == client._nick)
        {
             _channels.at(params[0])->_topic = message;
            sendServerToChannel(params[0], RPL_TOPIC(client._nick,params[0],message));
        }
        else if ( _channels.at(params[0])->_mode == ProtectedTopic)
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, params[0]));
        else if(_channels.at(params[0])->_topic.empty() || message.empty() || message == " ")
        {
            _channels.at(params[0])->_topic = "";
            sendServerToChannel(params[0], RPL_NOTOPIC(client._nick, params[0]));
        }
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
