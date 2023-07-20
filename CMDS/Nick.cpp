#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "Replies.hpp"


//ERR_NONICKNAMEGIVEN()
//ERR_ERRONEUSNICKNAME(Nick)
//ERR_NICKNAMEINUSE (Nick)
void Server::Nick(Client &client, std::vector<const std::string &> params)
{
    if (int err = ParamsSizeControl(params, 1) != 0)
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
    case RegistrationState::PassRegistered:
        client._nick = ToLowercase(params[0]);
        client._status = RegistrationState::NickRegistered;
        // nick assigned yazdır?;
        break;
    default:
        std::string old_nick = client._nick;
        client._nick = ToLowercase(params[0]);
        sendClientToChannel(client, client._channel->_name, old_nick + ": changed nickname to " + client._nick);
        break;
    }
}
