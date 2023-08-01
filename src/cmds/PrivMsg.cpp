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
 enum Prefix
 {
    PrefixClient,
    PrefixChannel,
    PrefixChannelOp,
 };
 
enum Prefix Server::PrefixControl(std::string str)
{
    std::string message;
    enum Prefix pre = PrefixClient;
    if(!str.empty() && (str[0] == '@') && str[1] == '#')
        pre = PrefixChannelOp;
    else if(!str.empty() && str[0] == '#')
        pre = PrefixChannel;
    return pre;
}

void Server::PrivMsg(class Client &client, std::vector<std::string > params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    size_t count = params.size();
    if(count == 1)
    {
        sendServerToClient(client,ERR_NOTEXTTOSEND(client._nick));
        return ;
    }
    else if(count == 0)
    {
        sendServerToClient(client, ERR_NORECIPIENT(client._nick, std::string("PRIVMSG")));
        return ;
    }
    enum Prefix pre = PrefixControl(params[0]);
    std::string message;
    message = params[1].substr(1);
    for (size_t i = 0; i < count - 2; i++)
            message += " " + params[i];
    switch (pre)
    {
    case PrefixClient:
        if(IsExistClient(params[0],0))
        {
            Client& toClient = findClient(params[0]);
            sendServerToClient(toClient, ":" + client._nick + " PRIVMSG " + toClient._nick + " :" + message);
        }
        else
            sendServerToClient(client,ERR_NOSUCHNICK(client._nick, params[0]));
        break;
    case PrefixChannelOp:
        if (IsExistChannel(params[1].substr(1)) && !IsBannedClient(client,params[0]))
            sendServerToClient(*_channels.at(params[1].substr(1))->getOperator(), ":" + client._nick + " PRIVMSG " + params[1].substr(1) + " :" + message);
        else if(IsBannedClient(client,params[1].substr(1)))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[1].substr(1)));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick,params[1].substr(1)));
        break;
    case PrefixChannel:
        if(IsExistChannel(params[0]) && IsInChannel(client,params[0]) && !IsBannedClient(client,params[0]))
            sendClientToChannel(client,params[0], ":" + client._nick + " PRIVMSG " + params[0] + " :" + message);
        else if(!IsInChannel(client,params[0]) || IsBannedClient(client,params[0]))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[0]));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick, params[0]));
        break;
    default:
        break;
    }
}
