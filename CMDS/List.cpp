#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

void Server::List(class Client &, std::vector<const std::string &> params)
{
    if(params[0].empty())
    {
        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            std::cout << "channelname: " << it->first << "limit: " << it->second._clientLimit << ", operator: @" << it->second.getOperator()->_nick << ", topic: " << it->second._topic << "\n";
        }
        return ;
    }
    if (IsExistChannel(params[0]))
    {
        std::cout << "channelname: " << params[0] << "limit: " << _channels[params[0]]._clientLimit << ", operator: @" << _channels[params[0]].getOperator()->_nick << ", topic: " << _channels[params[0]]._topic << "\n";
    }
    else
        ; // print channel not exist
}