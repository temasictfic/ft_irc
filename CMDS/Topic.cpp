#include "Server.hpp"
#include "Utils.hpp"
#include "Channel.hpp"
#include "Client.hpp"

void Server::Topic(Client &client, std::vector<const std::string &> params)
{
    if (InvalidPrefix(params[1]) || InvalidLetter(params[1]))
        // print error doesnt allow this letter
        if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
        {
            if (_channels[params[0]]._mode == Mode::ProtectedTopic && _channels[params[0]].getOperator()->_nick == client._nick)
                _channels[params[0]]._topic = params[1];
            else if (_channels[params[0]]._mode == Mode::ProtectedTopic)
            {
                // print error
            }
            else
                _channels[params[0]]._topic = params[1];
            ;
        }
}