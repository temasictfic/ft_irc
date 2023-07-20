#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
#include "Utils.hpp"
#include <unistd.h>

// NONE

void Server::Quit(Client &client, std::vector<const std::string &> params)
{
    if(sendClientToChannel(client,client._channel->_name," is out now.\n") != 0)
        client._channel->removeMembers(client);

    // remove from clients.
    _clients.erase(_clients.begin() + client._idx);
    close(client.getSocketFd());
}
