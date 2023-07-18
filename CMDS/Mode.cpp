#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Mode(Client &client, std::vector<const std::string &> params)
{
    if (IsExistChannel(params[0]) && _channels[params[0]].getOperator()->_nick == client._nick)
    {
        if (params[1].size() == 2)
        {
            ChangeMode(_channels[params[0]]._mode, params[1]);
        }
        else
            ; // print mode char error
    }
    else if (IsExistChannel(params[0]))
    {
        ; // print not operator msg.
    }
    else
        ; // print channel does not exist msg.
}