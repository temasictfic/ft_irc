#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

void Server::Join(Client &client,std::vector<const std::string &> params)
{
    if (IsExistChannel(params[0]))
    {
        if (IsInChannel(client, params[0]))
        {
            // print client already in channel msg;
        }

        else if (IsBannedClient(client, params[0]))
        {
            // print client cannot join bcs in banned list of channel msg;
        }
        else if (IsChannelLimitFull(params[0]))
        {
            // Channel is full
        }
        else if (IsKeyWrong(params[0], params[1]))
        {
            // if key wrong print error
        }
        else
        {
            _channels[params[0]].addMembers(client);
        }
    }
    else
    {
        if (InvalidLetter(params[0]) || params[0][0] != '#')
        {
            // print invalid ChannelName error
        }
        else
        {
            Channel newish(params[0]);
            _channels.insert(std::pair<std::string, Channel>(params[0], newish));
            if(!params[1].empty())
                _channels[params[0]].setKey(params[1]);
        }
    }
}