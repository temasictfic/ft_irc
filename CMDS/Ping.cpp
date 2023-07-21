#include "Server.hpp"

//ERR_NEEDMOREPARAMS(Command)

void Server::Ping(Client &client, std::vector<const std::string&>)
{
    sendServerToClient(client, "Pong");
}
