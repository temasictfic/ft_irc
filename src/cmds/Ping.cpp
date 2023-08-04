#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)

void Server::Ping(Client &client, std::vector<std::string>)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));

    sendServerToClient(client, "Pong"); // params[0] as token?
}
