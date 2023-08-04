#include "../../inc/Server.hpp"


//ERR_NEEDMOREPARAMS (461)*
//ERR_NOSUCHCHANNEL (403)*
//ERR_NOTONCHANNEL (442)*

void Server::Part(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "PART", params, 1, 1) != 0)
        return;
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if(IsOperator(client, params[0]))
        {
            sendServerToChannel(params[0], PART(client._nick, params[0] + " :closed the channel"));
            for(std::vector<Client*>::iterator it = _channels.at(params[0])->getMembers().begin(); it != _channels.at(params[0])->getMembers().end(); it++)
                (*it)->_channel.erase(params[0]);
            _channels[params[0]]->setOperator(NULL);
            _channels.erase(params[0]);
/*             Channel* puf = _channels.at(params[0]);
            _channels.erase(puf->_name);
            delete puf; */
        }
        else
        {
            sendServerToChannel(params[0], PART(client._nick, params[0]));
            _channels.at(params[0])->removeMembers(client);
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(client._nick, params[0]));
    }
}
