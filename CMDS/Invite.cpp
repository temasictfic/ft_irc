#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Invite(Client &client, std::vector<const std::string &> params)
{
    if (params[0].empty() || params[1].empty())
    {}
        // print error
        // check there is a man with this nick in clients
        if (IsExistClient(params[1], 0) && (IsExistChannel(params[0]) && IsInChannel(client, params[0])))
        {
            if (_channels.at(params[0])._mode == Mode::InviteOnly && _channels.at(params[0]).getOperator()->_nick == client._nick)
                _channels.at(params[0]).addMembers(findClient(params[1]));
            else if (_channels[params[0]]._mode == Mode::InviteOnly)
            {
                // print error
            }
            else
                _channels.at(params[0]).addMembers(findClient(params[1]));
        }
}
