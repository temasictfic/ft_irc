#include "../../inc/Server.hpp"

//ERR_NONICKNAMEGIVEN()
//ERR_ERRONEUSNICKNAME(Nick)
//ERR_NICKNAMEINUSE (Nick)


/*  NICK Wiz                  ; Requesting the new nick "Wiz".
Message Examples:

:WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy. */

//(:oldnick NICK newnick) sendServertoChannel(source)
void Server::Nick(Client &client, std::vector<std::string> params)
{
    if (client._status == None)
        return sendServerToClient(client, ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "NICK", params, 1, 0) != 0)
        return;
    if (InvalidLetter(params[0]) || InvalidPrefix(params[0]))
        sendServerToClient(client, ERR_ERRONEUSNICKNAME(params[0]));
    else if (IsExistClient(params[0]))
        sendServerToClient(client, ERR_NICKNAMEINUSE(params[0]));
    switch (client._status)
    {
    case PassRegistered:
        client._nick = ToLowercase(params[0]);
        client._username = client._nick;
        client._realname = client._nick;
        client._status = NickRegistered;
        std::cout << "Nick assigned" << "\n";
        break;
    default:
        std::string old_nick = client._nick;
        client._nick = ToLowercase(params[0]);
        std::cout << "Nick changed" << "\n";
        for (std::map<std::string, Channel*>::iterator chan = client._channel.begin(); chan != client._channel.end(); chan++)
        {
            sendClientToChannel(client, chan->first, ":" + old_nick + " NICK " + client._nick);
        }
    }
}
