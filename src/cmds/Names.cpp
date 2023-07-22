#include "../../inc/Server.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Replies.hpp"
//RPL_NAMREPLY (353)
//RPL_ENDOFNAMES (366)

void Server::Names(class Client & client,std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1, 0) != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/LIST")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/LIST Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]))
    {
       sendServerToClient(client, "@" +  _channels.at(params[0]).getOperator()->_nick + '\n');
        for (std::vector<Client>::iterator it =  _channels.at(params[0]).getMembers().begin(); it !=  _channels.at(params[0]).getMembers().end(); it++)
        {
            sendServerToClient(client,it->_nick + "\n");
        }
    }
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
}
