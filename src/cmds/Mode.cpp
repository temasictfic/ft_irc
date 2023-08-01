#include "../../inc/Server.hpp"

//ERR_NOSUCHNICK (401)-
//If <modestring> is not given, the RPL_UMODEIS (221) numeric 
//is sent back containing the current modes of the target user.
//ERR_UNKNOWNMODE (472)*
//ERR_NOSUCHCHANNEL (403)*
//If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned. 
//ERR_CHANOPRIVSNEEDED (482)*

//Ban List "+b": Ban lists are returned with zero or more RPL_BANLIST (367) numerics,
// followed by one RPL_ENDOFBANLIST (368) numeric.


void Server::Mode(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    std::map<char, int> modes;
    modes['d'] = Default;
    modes['i'] = InviteOnly;
    modes['k'] = KeyChannel;
    modes['t'] = ProtectedTopic;
    //modes['b'] = Ban;
    //modes['l'] = ChanneLimit;
    //modes['o'] = ChanOp;


    int err = ParamsSizeControl(params, 1, 2);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("MODE")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("MODE"), std::string("Excessive argument is given")));
        return;
    }
    if (IsExistChannel(params[0]) && _channels.at(params[0])->getOperator()->_nick == client._nick)
    {
        if (params.size() == 1)
        {
            std::string msg = params[0] + " modes are: +";
            for (std::map<char, int>::iterator it = modes.begin(); it != modes.end(); it++)
                if (_channels.at(params[0])->_mode & it->second)
                    msg += it->first;
            sendServerToClient(client, msg);
        }
        else if (params[1].size() == 2)
            ChangeMode(client, params, modes) ? sendServerToChannel(client._channel.at(params[0])._name,"Mode successfully changed as " + params[1]) : sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
        else
            sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
    }
    else if (IsExistChannel(params[0]))
       sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, client._channel.at(params[0])._name));
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
}
