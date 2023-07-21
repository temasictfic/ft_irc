#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Replies.hpp"

//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)*
//ERR_NOTONCHANNEL (442)*

void Server::Part(Client &client, std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1, 0) != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PART")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/PART Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if(_channels[params[0]].getOperator()->_nick == client._nick)
        {
            sendServerToChannel(params[0], "operator" + client._nick + ": is part" + params[0] + "\n");
            for(std::vector<Client>::iterator it = _channels.at(params[0]).getMembers().begin(); it != _channels.at(params[0]).getMembers().end(); it++)
                it->_channel = nullptr;
            _channels[params[0]].setOperator(nullptr);
            _channels.erase(params[0]);
        }
        else if(IsInChannel(client,params[0]))
        {
            sendServerToChannel(params[0], client._nick + ": is out from" + params[0] + "\n");
            _channels.at(params[0]).removeMembers(client);
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(params[0]));
    }
}
