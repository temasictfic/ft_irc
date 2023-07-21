#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
#include "Utils.hpp"
#include <unistd.h>

// NONE*

void Server::Quit(Client &client, std::vector<const std::string &> params)
{
    // apart from channel.
    Part(client, std::vector<const std::string&>(1, client._channel->_name));
    // remove from clients.
    _clients.erase(_clients.begin() + client._idx);
    close(client.getSocketFd());
}
