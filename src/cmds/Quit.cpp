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
    if (client._channel.find(params[0]) != client._channel.end())
    {
        std::vector<std::string> par;
        par.push_back(client._channel.at(params[0])._name);
        Part(client, par);
    }
    // make client offline.
    client._online = false;
}
