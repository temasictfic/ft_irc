#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
//RPL_NAMREPLY (353)
//RPL_ENDOFNAMES (366)

void Server::Names(class Client & client,std::vector<const std::string &> params)
{
    if(ParamsSizeControl(params,1))
     {
        sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/NAMES")));
        return ;
     }
    if (IsExistChannel(params[0]))
    {
        std::cout << '@' << _channels[params[0]].getOperator()->_nick << '\n';
        for (std::vector<Client>::iterator it = _channels[params[0]].getMembers().begin(); it != _channels[params[0]].getMembers().end(); it++)
        {
            std::cout << it->_nick << "\n";
        }
    }
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
}