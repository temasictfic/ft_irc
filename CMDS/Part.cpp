#include "Server.hpp"
#include "Channel.hpp"

void Server::Part(Client &client, std::vector<const std::string &> params)
{
    // print params[1] as reason
    _channels[params[0]].removeMembers(client);
}
