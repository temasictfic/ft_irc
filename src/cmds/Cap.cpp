#include "../../inc/Server.hpp"

//CAP LS 302
void Server::Cap(Client &client, std::vector<std::string> params)
{
    if (params[0] == "LS")
    {
        sendServerToClient(client, RPL_ISUPPORT(client._nick));
        sendServerToClient(client, CAP_LS);
    }
    else if (params[0] == "END" && client._status == UsernameRegistered)
        sendServerToClient(client, RPL_WELCOME(client._nick, client._username));
}