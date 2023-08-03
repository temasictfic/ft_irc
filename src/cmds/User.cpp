#include "../../inc/Server.hpp"

//ERR_NEEDMOREPARAMS(Command)
//ERR_ALREADYREGISTERED()
//RPL_WELCOME()


//USER Bit 0 * :realname
void Server::User(Client &client, std::vector<std::string>params)
{
    if (client._status < 2)
    {
        sendServerToClient(client, ERR_NOTREGISTERED(client._nick));
        return;
    }
    int err = ParamsSizeControl(params, 3, 2);
    if ( err != 0)  
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("USER")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("USER"), std::string("Excessive argument is given")));
        return;
    }
    size_t count = params.size();
    switch (client._status)
    {
    case NickRegistered:
        client._username = params[0];
        if(count > 3)
        {
            client._realname = params[3].substr(1);
            for (size_t i = 0; i < count - 4; i++)
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
            for (size_t i = 0; i < count - 4; i++)
                client._realname += " " + params[i];
        }
        std::cout << "Username changed" << "\n";
        break;
    default:
        break;
    }
}