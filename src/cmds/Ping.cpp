#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)

void Server::Ping(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (IsExistClient(params[0]))
    {
        std::vector<std::string> pong;
        pong.push_back("PONG " + params[2]);
        PrivMsg(findClient(params[0]), pong);
    }
    else
        sendServerToClient(client, ":ircserv " + params[0]); 
}
