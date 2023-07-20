#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
//When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer,
// and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

//RPL_INVITING (341)
//ERR_NEEDMOREPARAMS (461)
//ERR_NOSUCHCHANNEL (403)
//ERR_NOTONCHANNEL (442)
//ERR_CHANOPRIVSNEEDED (482)
//ERR_USERONCHANNEL (443)
//ERR_NOSUCHNICK (401)


void Server::Invite(Client &client, std::vector<const std::string &> params)
{
    // check there is a man with this nick in clients
    if(ParamsSizeControl(params,2))
    {
        sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/INVITE")));
        return ;
    }
    if (IsExistClient(params[1], 0) && (IsExistChannel(params[0]) && IsInChannel(findClient(params[1]), params[0])))
    {
        if (_channels.at(params[0])._mode == Mode::InviteOnly && _channels.at(params[0]).getOperator()->_nick == client._nick)
            _channels.at(params[0]).addMembers(findClient(params[1]));
        else if(IsBannedClient(findClient(params[1]),params[0]) && _channels.at(params[0]).getOperator()->_nick == client._nick)
        {
            _channels.at(params[0]).addMembers(findClient(params[1]));
            _channels.at(params[0]).removeBanned(findClient(params[1]));
        }
        else
            _channels.at(params[0]).addMembers(findClient(params[1]));
    }
    else
    {
        if(!IsExistClient(params[1],0))
            sendServerToClient(client, ERR_NOSUCHNICK(params[1]));
        else if(!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
        else if(!IsInChannel(findClient(params[1]), params[0]))
            sendServerToClient(client, ERR_USERONCHANNEL(params[1],params[0]));

    }
}