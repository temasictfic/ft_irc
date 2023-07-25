#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Replies.hpp"
#include "../../inc/Server.hpp"
//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer,
// and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

//RPL_INVITING (341)*
//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403) *
//ERR_NOTONCHANNEL (442) *
//ERR_CHANOPRIVSNEEDED (482)*
//ERR_USERONCHANNEL (443)*
//ERR_NOSUCHNICK (401) *


void Server::Invite(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    int err = ParamsSizeControl(params, 2, 0);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/INVITE")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/INVITE Excessive argument is given")));
        return;
    }
    if (IsInChannel(client,params[0]) && IsExistClient(params[1], 0) && IsExistChannel(params[0]))
    {
        Client& invited = findClient(params[1]);
        if (IsInChannel(invited, params[0]))
        {
            sendServerToClient(client, ERR_USERONCHANNEL(invited._nick, params[0]));
            return;
        }
        std::vector<std::string> channel;
        channel.push_back(params[0]);
        if (_channels.at(params[0])->_mode == InviteOnly && _channels.at(params[0])->getOperator()->_nick == client._nick)  
            Join(invited, channel);
        else if (_channels.at(params[0])->_mode == InviteOnly)
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(params[0]));
        else if(IsBannedClient(invited,params[0]) && _channels.at(params[0])->getOperator()->_nick == client._nick)
        {
            _channels.at(params[0])->removeBanned(invited);
            Join(invited, channel);
        }
        else
            Join(invited, channel);
    }
    else
    {
        if(!IsExistClient(params[1],0))
            sendServerToClient(client, ERR_NOSUCHNICK(params[1]));
        else if(!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
        else if(!IsInChannel(client,params[0]))
            sendServerToClient(client,  ERR_NOTONCHANNEL(params[0]));
    }
}
