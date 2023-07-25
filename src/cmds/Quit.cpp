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
    if (client._channel)
    {
        std::vector<std::string> par;
        par.push_back(client._channel->_name);
        Part(client, par);
    }
    // make client offline.
    client._online = false;
}
