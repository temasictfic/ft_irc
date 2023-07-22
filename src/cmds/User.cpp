#include "../../inc/Server.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Replies.hpp"
#include "../../inc/Utils.hpp"

//ERR_NEEDMOREPARAMS(Command)
//ERR_ALREADYREGISTERED()
//RPL_WELCOME()

void Server::User(Client &client, std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1, 1) != 0)  
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/USER")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/USER Excessive argument is given")));
        return;
    }
    switch (client._status)
    {
    case RegistrationState::NickRegistered:
        client._username = params[0];
        if(params.size() > 1)
            client._realname = params[1];
        client._status = RegistrationState::UsernameRegistered;
        // username assigned yazdır?;
        sendServerToClient(client, RPL_WELCOME(client._nick));
        break;
    case RegistrationState::UsernameRegistered:
        client._username = params[0];
        if(params.size() > 1)
            client._realname = params[1];
        break;
    }
}