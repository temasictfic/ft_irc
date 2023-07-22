#include "../../inc/Server.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Replies.hpp"
#include "../../inc/Utils.hpp"
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
