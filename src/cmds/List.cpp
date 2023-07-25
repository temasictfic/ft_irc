#include "../../inc/Server.hpp"
#include <sstream>

//RPL_LISTSTART (321)
//RPL_LIST (322)
//RPL_LISTEND (323)

void Server::List(class Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    if(params[0].empty())
    {
        sendServerToClient(client, "List of Channels: ");
        for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            std::ostringstream limit;
            limit << it->second->_clientLimit;
            sendServerToClient(client,"channelname: " + it->first + ", limit: " + limit.str() + ", operator: @" + it->second->getOperator()->_nick + ", topic: " + it->second->_topic);
        }
        return ;
    }
    int err = ParamsSizeControl(params, 0, 1);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/LIST")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/LIST Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]))
    {
        std::ostringstream limit;
        limit << _channels.at(params[0])->_clientLimit;
        sendServerToClient(client,"channelname: " + params[0] + " limit: " + limit.str() + ", operator: @" +  _channels.at(params[0])->getOperator()->_nick + ", topic: " +  _channels.at(params[0])->_topic + "\n");
    }
    else
       sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
}
