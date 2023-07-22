#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Replies.hpp"
//RPL_LISTSTART (321)
//RPL_LIST (322)
//RPL_LISTEND (323)

void Server::List(class Client &client, std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1, 0) != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/LIST")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/LIST Excessive argument is given")));
        return;
    }
    if(params[0].empty())
    {
        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
             sendServerToClient(client,"channelname: " + it->first + "limit: " + std::to_string(it->second._clientLimit) + ", operator: @" + it->second.getOperator()->_nick + ", topic: " + it->second._topic + "\n");
        }
        return ;
    }
    if (IsExistChannel(params[0]))
        sendServerToClient(client,"channelname: " + params[0] + "limit: " + std::to_string( _channels.at(params[0])._clientLimit) + ", operator: @" +  _channels.at(params[0]).getOperator()->_nick + ", topic: " +  _channels.at(params[0])._topic + "\n");
    else
       sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
}
