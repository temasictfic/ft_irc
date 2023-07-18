#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Kick(Client &client, std::vector<const std::string &> params)
{
    if(params.empty())
    {
        // print error
        return ;
    }
    if(client._channel && client._channel->getOperator()->_nick == client._nick)
    {
        if(IsExistClient(params[0],0))
        {
            Client &ban(findClient(params[0]));
            client._channel->removeMembers(ban);
            client._channel->addBanned(ban);
        }
    }
}
