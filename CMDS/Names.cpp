#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Names(class Client & client,std::vector<const std::string &> params)
{
    if (IsExistChannel(params[0]))
    {
        std::cout << '@' << _channels[params[0]].getOperator()->_nick << '\n';
        for (std::vector<Client>::iterator it = _channels[params[0]].getMembers().begin(); it != _channels[params[0]].getMembers().end(); it++)
        {
            std::cout << it->_nick << "\n";
        }
    }
}