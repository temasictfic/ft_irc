#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS (461) *
//ERR_NOSUCHCHANNEL (403) *
//ERR_CHANOPRIVSNEEDED (482)*
//ERR_USERNOTINCHANNEL (441)*
//ERR_NOTONCHANNEL (442)*


void Server::Kick(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    int err = ParamsSizeControl(params, 2, 0);
     if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("KICK")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("KICK"), std::string("Excessive argument is given")));
        return;
    }
    if(IsExistChannel(params[0]) && IsExistClient(params[1]) && IsInChannel(client, params[0]) && client._channel.at(params[0])->getOperator()->_nick == client._nick)
    {
        Client &kicked = findClient(params[1]);
        if(IsInChannel(kicked, params[0]) &&  client._channel.at(params[0])->getOperator()->_nick != kicked._nick)
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
