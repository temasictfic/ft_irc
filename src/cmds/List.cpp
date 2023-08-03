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
        std::ostringstream count;
        for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            count << it->second->getMembers().size();
            sendServerToClient(client, RPL_LIST(client._nick, params[0], count.str(), it->second->_topic));
            count.clear();
        }
        sendServerToClient(client, RPL_LISTEND(client._nick));
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
        std::ostringstream count;
        count << _channels.at(params[0])->getMembers().size();
        sendServerToClient(client, RPL_LIST(client._nick, params[0], count.str(), _channels.at(params[0])->_topic));
        sendServerToClient(client, RPL_LISTEND(client._nick));
    }
    else
       sendServerToClient(client, RPL_LISTEND(client._nick));
}
