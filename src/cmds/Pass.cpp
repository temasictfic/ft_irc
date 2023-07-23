#include "../../inc/Server.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Utils.hpp"
#include "../../inc/Replies.hpp"

// ERR_NEEDMOREPARAMS(Command)
// ERR_ALREADYREGISTERED()
// ERR_PASSWDMISMATCH()
void Server::Pass(Client &client, std::vector<std::string> params)
{
    int err = ParamsSizeControl(params, 1, 0);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PASS")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/PASS Excessive argument is given")));
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
            sendServerToClient(client, ERR_PASSWDMISMATCH());
        break;
    default:
        sendServerToClient(client, ERR_ALREADYREGISTERED());
    }
}
