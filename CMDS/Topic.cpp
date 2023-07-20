#include "Server.hpp"
#include "Utils.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Replies.hpp"

//ERR_NEEDMOREPARAMS (461)
//ERR_NOSUCHCHANNEL (403)
//ERR_NOTONCHANNEL (442)
//ERR_CHANOPRIVSNEEDED (482)
//RPL_NOTOPIC (331)
//RPL_TOPIC (332)

void Server::Topic(Client &client, std::vector<const std::string &> params)
{
    if(int err = ParamsSizeControl(params,1) != 0)
    {   
        if (err == -1)
            sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/TOPIC")));
        else if (err == 1)
            sendServerToClient(client, ERR_CUSTOM(std::string("/TOPIC Excessive argument is given")));
        return ;
    }
    if (InvalidPrefix(params[1]) || InvalidLetter(params[1]))
        sendServerToClient(client, ERR_CUSTOM(std::string("Wrong Prefix and letter use in Topic.\n")));
    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if (_channels[params[0]]._mode == Mode::ProtectedTopic && _channels[params[0]].getOperator()->_nick == client._nick)
            _channels[params[0]]._topic = params[1];
        else if (_channels[params[0]]._mode == Mode::ProtectedTopic)
               sendServerToClient(client, ERR_CHANOPRIVSNEEDED(params[0]));
        else
            _channels[params[0]]._topic = params[1];
    }
}