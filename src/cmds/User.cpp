#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)
//ERR_ALREADYREGISTERED()
//RPL_WELCOME()


//USER Bit 0 * :realname
void Server::User(Client &client, std::vector<std::string>params)
{
    if (client._status < NickRegistered)
        return sendServerToClient(client, ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "USER", params, 3, 2) != 0)  
        return;
    size_t count = params.size();
    switch (client._status)
    {
    case NickRegistered:
        client._username = params[0];
        if(count > 3)
        {
            client._realname = params[3].substr(1);
            for (size_t i = 4; i < count; i++)
                client._realname += " " + params[i];
        }
        client._status = UsernameRegistered;
        std::cout << "Username assigned" << "\n";
        break;
    case UsernameRegistered:
        client._username = params[0];
        if(count > 3)
        {
            client._realname = params[3].substr(1);
            for (size_t i = 4; i < count; i++)
                client._realname += " " + params[i];
        }
        std::cout << "Username changed" << "\n";
        break;
    default:
        break;
    }
}