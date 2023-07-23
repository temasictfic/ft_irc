#include "../../inc/Server.hpp"

void Server::Notice(class Client & server, std::vector<std::string> params)
{
    (void)server;
    std::cout << "I have noticed." << params[0] <<"\n";
/*     if(params[0][0] == '#')
        sendServerToChannel(bot, params[0]);
    else
        sendServerToClient() */
}
