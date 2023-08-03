#include "../../inc/Server.hpp"

// NONE*

void Server::Quit(Client &client, std::vector<std::string>)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    // apart from channel.
    if (!client._channel.empty())
    {
        for (std::map<std::string, Channel*>::iterator it = client._channel.begin(); it != client._channel.end(); it++)
        {
            std::vector<std::string> par;
            par.push_back(it->first);
            Part(client, par);
            par.clear();      
        }  
    }
    // make client offline.
    client._online = false;
}
