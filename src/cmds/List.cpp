#include "../../inc/Server.hpp"
#include <sstream>

//RPL_LISTSTART (321)
//RPL_LIST (322)*
//RPL_LISTEND (323)*

void Server::List(class Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if(params[0] == "")
    {
        std::ostringstream count;
        for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            count << it->second->getMembers().size();
            sendServerToClient(client, RPL_LIST(client._nick, it->first, count.str(), it->second->_topic));
            count.clear();
        }
        return sendServerToClient(client, RPL_LISTEND(client._nick));
    }
    if (ParamsSizeControl(client, "LIST", params, 0, 1) != 0)
        return;
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
