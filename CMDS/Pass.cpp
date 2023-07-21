#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "Replies.hpp"

// ERR_NEEDMOREPARAMS(Command)
// ERR_ALREADYREGISTERED()
// ERR_PASSWDMISMATCH()
void Server::Pass(Client &client, std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1, 0) != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PASS")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/PASS Excessive argument is given")));
        return;
    }
    if (PasswordMatched(this->getPassword(), params[0]))
    {
        switch (client._status)
        {
        case RegistrationState::None:
            client._status = RegistrationState::PassRegistered;
            //password doğru alındı yazdırılabilinir.
            break;
        default:
            sendServerToClient(client, ERR_ALREADYREGISTERED());
        }
    }
    else
        sendServerToClient(client, ERR_PASSWDMISMATCH());
}
