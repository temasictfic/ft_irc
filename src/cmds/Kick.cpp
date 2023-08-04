#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS (461) *
//ERR_NOSUCHCHANNEL (403) *
//ERR_CHANOPRIVSNEEDED (482)*
//ERR_USERNOTINCHANNEL (441)*
//ERR_NOTONCHANNEL (442)*


void Server::Kick(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "KICK", params, 2, 0) != 0)
        return;
    if(IsExistChannel(params[0]) && IsExistClient(params[1]) && IsInChannel(client, params[0]) && IsOperator(client, params[0]))
    {
        Client &kicked = findClient(params[1]);
        if(IsInChannel(kicked, params[0]) &&  !IsOperator(kicked, params[0]))
        {
            sendServerToChannel(params[0], KICK(client._nick, params[0], kicked._nick));
            client._channel.at(params[0])->removeMembers(kicked);
        }
        else
            sendServerToClient(client, ERR_USERNOTINCHANNEL(client._nick, params[1], params[0]));
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(client._nick, params[0]));
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, client._channel.at(params[0])->_name));
    }
}
