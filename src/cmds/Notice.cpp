#include "../../inc/Server.hpp"

void Server::Notice(class Client & server, std::vector<std::string> params)
{
    PrivMsg(server, params);
}
