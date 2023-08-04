#include "../../inc/Server.hpp"

// ERR_NEEDMOREPARAMS(Command)*
// ERR_ALREADYREGISTERED()*
// ERR_PASSWDMISMATCH()*
void Server::Pass(Client &client, std::vector<std::string> params)
{
    if (ParamsSizeControl(client, "PASS", params, 1, 0) != 0)
        return;
    switch (client._status)
    {
    case None:
        if (PasswordMatched(this->getPassword(), params[0]))
        {
            client._status = PassRegistered;
            std::cout << "Pass assigned" << "\n";
        }
        else
            sendServerToClient(client, ERR_PASSWDMISMATCH(client._nick));
        break;
    default:
        sendServerToClient(client, ERR_ALREADYREGISTERED(client._nick));
    }
}
