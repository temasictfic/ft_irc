#include "../../inc/Server.hpp"

// ERR_NEEDMOREPARAMS(Command)
// ERR_ALREADYREGISTERED()
// ERR_PASSWDMISMATCH()
void Server::Pass(Client &client, std::vector<std::string> params)
{
    int err = ParamsSizeControl(params, 1, 0);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("PASS")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("PASS"), std::string("Excessive argument is given")));
        return;
    }
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
