#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
//ERR_NEEDMOREPARAMS (461)
//ERR_NOSUCHCHANNEL (403)
//ERR_CHANOPRIVSNEEDED (482)
//ERR_USERNOTINCHANNEL (441)
//ERR_NOTONCHANNEL (442)


void Server::Kick(Client &client, std::vector<const std::string &> params)
{
     if(ParamsSizeControl(params,1))
     {
         sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/KICK")));
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
        else
            sendServerToClient(client, ERR_NOSUCHNICK(params[0]));
    }
    else
        sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._channel->_name));
}
