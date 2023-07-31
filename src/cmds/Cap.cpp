#include "../../inc/Server.hpp"
#define CAP_LS "ircserv: CAP * LS :"

//CAP LS 302
void Server::Cap(Client &client, std::vector<std::string> params)
{
    if (params[0] == "LS")
    {
        
    }
        sendServerToClient(client, CAP_LS);
    else if (params[0] == "END" && client._status == UsernameRegistered)
        sendServerToClient(client, RPL_WELCOME(client._nick));
}