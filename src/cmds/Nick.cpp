#include "../../inc/Server.hpp"

//ERR_NONICKNAMEGIVEN()
//ERR_ERRONEUSNICKNAME(Nick)
//ERR_NICKNAMEINUSE (Nick)
void Server::Nick(Client &client, std::vector<std::string> params)
{
    if (client._status == None)
    {
        sendServerToClient(client, ERR_NOTREGISTERED());
        return;
    }
    int err = ParamsSizeControl(params, 1, 0);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/NICK")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/NICK Excessive argument is given")));
        return;
    }
    if (InvalidLetter(params[0]) || InvalidPrefix(params[0]))
        sendServerToClient(client, ERR_ERRONEUSNICKNAME(params[0]));
    else if (IsExistClient(params[0], 0))
        sendServerToClient(client, ERR_NICKNAMEINUSE(params[0]));
    switch (client._status)
    {
    case PassRegistered:
        client._nick = ToLowercase(params[0]);
        client._username = ToLowercase(params[0]);
        client._realname = ToLowercase(params[0]);
        client._status = NickRegistered;
        std::cout << "Nick assigned" << "\n";
        break;
    default:
        std::string old_nick = client._nick;
        client._nick = ToLowercase(params[0]);
        std::cout << "Nick changed" << "\n";
        //sendClientToChannel(client, client._channel->_name, old_nick + ": changed nickname to " + client._nick);
    }
}
