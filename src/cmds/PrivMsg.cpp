#include "../../inc/Server.hpp"

//If <target> is a channel name and the client is banned
//the message will not be delivered and the command will silently fail.
//ERR_CANNOTSENDTOCHAN (404)

//The PRIVMSG message is sent from the server to client to deliver a message to that client. 
//The <source> of the message represents the user or server that sent the message, and the <target> represents the target of that PRIVMSG

//ERR_NOSUCHNICK (401)
//ERR_NORECIPIENT (411)
//ERR_NOTEXTTOSEND (412)

 //PRIVMSG @#bunny :Hi! I have a problem!  //Send to chanel op of bunny
 //PRIVMSG Angel :yes I'm receiving it !  //Send to nickname Angel

void Server::PrivMsg(class Client &client, std::vector<std::string > params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    if(params.size() < 2)
    {
        sendServerToClient(client,ERR_NOTEXTTOSEND());
        return ;
    }
    else if(params.empty())
    {
        sendServerToClient(client, ERR_NORECIPIENT(std::string("/PRIVMSG.")));
        return ;
    }
    std::string message;
    for(size_t index = 1; index < params.size(); index++)
        message += params[index]; 
    if(!IsExistClient(params[0],0))
    {
        sendServerToClient(client,ERR_NOSUCHNICK(params[0]));
        return ;
    }
    else if(params[0][0] == '#' && !IsExistChannel(params[0]))
    {
        sendServerToClient(client,ERR_NOSUCHCHANNEL(params[0]));
        return ;
    }
    if (IsExistChannel(params[0]))
        sendClientToClient(client,*_channels.at(params[0])->getOperator(),message);
    else if (IsExistClient(params[0], 0))
       sendClientToClient(client,findClient(params[0]),message);
}
