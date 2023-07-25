#include "../../inc/Server.hpp"


//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)*
//ERR_NOTONCHANNEL (442)*

void Server::Part(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED());
        return ;
    }
    int err = ParamsSizeControl(params, 1, 0);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PART")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/PART Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if(_channels.at(params[0])->getOperator()->_nick == client._nick)
        {
            sendServerToChannel(params[0], "operator " + client._nick + ": closed the channel " + params[0]);
            for(std::vector<Client*>::iterator it = _channels.at(params[0])->getMembers().begin(); it != _channels.at(params[0])->getMembers().end(); it++)
                (*it)->_channel = NULL;
            _channels[params[0]]->setOperator(NULL);
           // delete _channels.at(params[0]);
            _channels.erase(params[0]);
        }
        else
        {
            sendServerToChannel(params[0], client._nick + ": is out from " + params[0]);
            _channels.at(params[0])->removeMembers(client);
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(params[0]));
    }
}
