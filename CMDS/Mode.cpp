#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
//ERR_NOSUCHNICK (401)
//If <modestring> is not given, the RPL_UMODEIS (221) numeric 
//is sent back containing the current modes of the target user.
//ERR_UMODEUNKNOWNFLAG (501)
//ERR_NOSUCHCHANNEL (403)
//If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned. 
//ERR_CHANOPRIVSNEEDED (482)

//Ban List "+b": Ban lists are returned with zero or more RPL_BANLIST (367) numerics,
// followed by one RPL_ENDOFBANLIST (368) numeric.



void Server::Mode(Client &client, std::vector<const std::string &> params)
{
    if(ParamsSizeControl(params,2))
     {
        sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/LIST")));
        return ;
     }
    if (IsExistChannel(params[0]) && _channels[params[0]].getOperator()->_nick == client._nick)
    {
        if (params[1].size() == 2)
            ChangeMode(_channels[params[0]]._mode, params[1]);
        else
            sendServerToClient(client,ERR_UNKNOWNMODE(params[1]));
    }
    else if (IsExistChannel(params[0]))
       sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._channel->_name));
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
}