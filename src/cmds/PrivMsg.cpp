#include "../../inc/Server.hpp"

//If <target> is a channel name and the client is banned
//the message will not be delivered and the command will silently fail.
//ERR_CANNOTSENDTOCHAN (404)*

//The PRIVMSG message is sent from the server to client to deliver a message to that client. 
//The <source> of the message represents the user or server that sent the message, and the <target> represents the target of that PRIVMSG

//ERR_NOSUCHNICK (401)*
//ERR_NORECIPIENT (411)*
//ERR_NOTEXTTOSEND (412)*

 //PRIVMSG @#bunny :Hi! I have a problem!  //Send to chanel op of bunny
 //PRIVMSG Angel :yes I'm receiving it !  //Send to nickname Angel


 // banlı ise or channelda değilse ERR_CANNOTSENDTOCHAN 

 //PRIVMSG @#wez
 
void Server::PrivMsg(class Client &client, std::vector<std::string > params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    size_t count = params.size();
    if(count == 1)
        return sendServerToClient(client,ERR_NOTEXTTOSEND(client._nick));
    else if (count == 0)
        return sendServerToClient(client, ERR_NORECIPIENT(client._nick, "PRIVMSG"));

    enum Prefix pre = PrefixControl(params[0]);
    std::string message = (params[1][0] == ':') ? params[1].substr(1) : params[1];
    for (size_t i = 2; i < count; i++)
            message += " " + params[i];
    switch (pre)
    {
    case PrefixClient:
        if(IsExistClient(params[0]))
        {
            Client& toClient = findClient(params[0]);
            sendServerToClient(toClient, PRIVMSG(client._nick, toClient._nick, message));
        }
        else
            sendServerToClient(client,ERR_NOSUCHNICK(client._nick, params[0]));
        break;
    case PrefixChannelOp:
        if (IsExistChannel(params[1].substr(1)) && !IsBannedClient(client,params[0]))
        {
            Client& op = *_channels.at(params[1].substr(1))->getOperator();
            sendServerToClient(op, PRIVMSG(client._nick, op._nick, message));
        }
        else if(IsBannedClient(client,params[1].substr(1)))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[1].substr(1)));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick,params[1].substr(1)));
        break;
    case PrefixChannel:
        if(IsExistChannel(params[0]) && IsInChannel(client,params[0]) && !IsBannedClient(client,params[0]))
            sendClientToChannel(client,params[0], PRIVMSG(client._nick, params[0], message));
        else if(!IsInChannel(client,params[0]) || IsBannedClient(client,params[0]))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[0]));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick, params[0]));
        break;
    default:
        break;
    }
}
