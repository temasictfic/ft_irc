#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)

void Server::Ping(Client &client, std::vector<std::string> params)
{
/*     if (IsExistClient(params[0]))
    {
        sendServerToClient(findClient(params[0]), ":" + client._nick + " PONG " + params[2]);
    }
    else */
        sendServerToClient(client, ":ircserv PONG " + params[0]); 
}
