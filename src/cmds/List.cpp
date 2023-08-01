#include "../../inc/Server.hpp"
#include <sstream>

//RPL_LISTSTART (321)
//RPL_LIST (322)
//RPL_LISTEND (323)

void Server::List(class Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    if(params.size() == 1)
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
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("LIST")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("LIST"), std::string("Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]))
    {
        std::ostringstream limit;
        limit << _channels.at(params[0])->_clientLimit;
        sendServerToClient(client,"channelname: " + params[0] + " limit: " + limit.str() + ", operator: @" +  _channels.at(params[0])->getOperator()->_nick + ", topic: " +  _channels.at(params[0])->_topic + "\n");
    }
    else
       sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
}
