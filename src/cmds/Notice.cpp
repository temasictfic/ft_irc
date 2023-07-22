#include "../../inc/Server.hpp"

void Server::Notice(class Client & server, std::vector<const std::string&> params)
{
    std::cout << "I have noticed." << "\n";
/*     if(params[0][0] == '#')
        sendServerToChannel(bot, params[0]);
    else
        sendServerToClient() */
}
