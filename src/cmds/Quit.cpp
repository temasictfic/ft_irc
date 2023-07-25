#include "../../inc/Server.hpp"

// NONE*

void Server::Quit(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    (void)params;
    // apart from channel.
    Part(client, std::vector<std::string>(1, client._channel->_name));
    // remove from clients.
    _clients.erase(_clients.begin() + client._idx);
    close(client.getSocketFd());
}
