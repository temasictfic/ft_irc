#include "../../inc/Server.hpp"

//RPL_NAMREPLY (353)*
//RPL_ENDOFNAMES (366)*

void Server::Names(class Client & client,std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "NAMES", params, 1, 0) != 0)
        return;
    if (IsExistChannel(params[0]))
    {
        std::string liststr = "@" +  _channels.at(params[0])->getOperator()->_nick;
        for (std::vector<Client*>::iterator it =  _channels.at(params[0])->getMembers().begin() + 1; it !=  _channels.at(params[0])->getMembers().end(); ++it)
        {
            liststr += " " + (*it)->_nick;
        }
        sendServerToClient(client, RPL_NAMREPLY(client._nick, params[0], liststr));
        sendServerToClient(client, RPL_ENDOFNAMES(client._nick, params[0]));
    }
    else
        sendServerToClient(client, RPL_ENDOFNAMES(client._nick, params[0]));
}
