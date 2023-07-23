#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)
//ERR_ALREADYREGISTERED()
//RPL_WELCOME()

void Server::User(Client &client, std::vector<std::string > params)
{
    if (client._status < 2)
    {
        sendServerToClient(client, ERR_NOTREGISTERED());
        return;
    }
    int err = ParamsSizeControl(params, 1, 1);
    if ( err != 0)  
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/USER")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/USER Excessive argument is given")));
        return;
    }
    switch (client._status)
    {
    case NickRegistered:
        client._username = params[0];
        if(params.size() > 1)
            client._realname = params[1];
        client._status = UsernameRegistered;
        std::cout << "Username assigned" << "\n";
        sendServerToClient(client, RPL_WELCOME(client._nick));
        break;
    case UsernameRegistered:
        client._username = params[0];
        if(params.size() > 1)
            client._realname = params[1];
        break;
    default:
        break;
    }
}