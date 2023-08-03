#include "../../inc/Server.hpp"

//ERR_NOSUCHNICK (401)*
//If <modestring> is not given, the RPL_UMODEIS (221)- numeric 
//is sent back containing the current modes of the target user.
//ERR_UNKNOWNMODE (472)*
//ERR_NOSUCHCHANNEL (403)*
//If <modestring> is not given, the RPL_CHANNELMODEIS (324)* numeric is returned. 
//ERR_CHANOPRIVSNEEDED (482)*

//Ban List "+b": Ban lists are returned with zero or more RPL_BANLIST (367) numerics,
// followed by one RPL_ENDOFBANLIST (368) numeric.


//MODE #foobar
//MODE #foobar -i
//MODE #foobar +i
//MODE #foobar -l
//MODE #foobar +l 20
//MODE #foobar -k
//MODE #foobar +k 123sfsg4
//MODE #foobar -b bunny
//MODE #foobar +b bunny
//MODE +o JOIN ile ilk channnel kurulunca
const std::map<char, int> ModeMap()
{
    static std::map<char, int> modes;
    modes['i'] = InviteOnly;
    modes['k'] = KeyChannel;
    modes['t'] = ProtectedTopic;
    modes['l'] = ChannelLimit;
    return modes;
}

void Server::Mode(Client &client, std::vector<std::string> params)
{
    if(client._status != UsernameRegistered)
    {
        sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
        return ;
    }
    const std::map<char, int>& modes = ModeMap();

    int err = ParamsSizeControl(params, 1, 2);
    if (err != 0)
    {
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, std::string("MODE")));
        else if (err == 1)
            sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, std::string("MODE"), std::string("Excessive argument is given")));
        return;
    }
    size_t count = params.size();
    if (IsExistChannel(params[0]))
    {
        std::string modestr = "+";
        for (std::map<char, int>::const_iterator it = modes.begin(); it != modes.end(); it++)
            if (_channels.at(params[0])->_mode & it->second)
                modestr += it->first;
        if (count == 1)
            sendServerToClient(client, RPL_CHANNELMODEIS(client._nick, params[0], modestr));
        if (client._nick == _channels.at(params[0])->getOperator()->_nick)
        {
            if(count == 2)
            {
                if (_channels.at(params[0])->ChangeModeTwoParams(params[1], modes))
                   sendServerToChannel(params[0], MODE(client._nick, params[0], modestr, "")); 
                else
                    sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
            }
            else if(count == 3)
            {
                if (_channels.at(params[0])->ChangeModeThreeParams(params[1], params[2], modes))
                    sendServerToChannel(params[0], MODE(client._nick, params[0], modestr, "")); 
                else if (IsExistClient(params[2]) && _channels.at(params[0])->ChangeBannedMode(findClient(params[2]),params[1], IsBannedClient(findClient(params[2]), params[0])))
                {
                    sendServerToChannel(params[0], MODE(client._nick, params[0], params[1], params[2])); 
                    std::vector<Client*>::iterator it = _channels.at(params[0])->getBanned().begin();
                    std::vector<Client*>::iterator end = _channels.at(params[0])->getBanned().end();
                    for (; it != end; it++)
                        sendServerToClient(client, RPL_BANLIST(client._nick, params[0], (*it)->_nick));
                    sendServerToClient(client, RPL_ENDOFBANLIST(client._nick, params[0]));
                }
                else if (!IsExistClient(params[2]))
                    sendServerToClient(client, ERR_NOSUCHNICK(client._nick, params[2]));
                else
                    sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
             }
        }
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, client._channel.at(params[0])->_name));
    }
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
}
