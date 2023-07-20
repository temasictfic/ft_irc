#include "Server.hpp"
#include "Replies.hpp"

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

void Server::PrivMsg(class Client &client, std::vector<const std::string &> params)
{
    if(ParamsSizeControl(params,1))
     {
         sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PRIVMSG")));
        return ;
     }
    if (IsExistChannel(params[0]))
    {
        //_channels[NickName].getOperator()->sendmessage(Message);
    }
    else if (IsExistClient(params[0], 0))
    {
        // findClient(NickName).sendmessage(Message);
    }
}