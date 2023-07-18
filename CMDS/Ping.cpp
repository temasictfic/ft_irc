#include "Server.hpp"

void Server::Ping(Client &client, std::vector<const std::string&>)
{
    sendServerToClient(client, "Pong");
}