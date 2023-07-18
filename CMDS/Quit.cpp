#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <unistd.h>

void Server::Quit(Client &client, std::vector<const std::string &> params)
{
    // print client nickname quited msg
    // remove client all the members list of channels joined
    client._channel->removeMembers(client);
    // remove from clients.
    _clients.erase(_clients.begin() + client._idx);
    close(client.getSocketFd());
}
