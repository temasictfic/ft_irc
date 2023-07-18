#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

void Server::Pass(Client &client, std::vector<const std::string &> params)
{
    if(!InvalidPassword(params[0]))
    {
     switch (client._status) // _status
        {
        case RegistrationState::None:
            client.setPassword(params[0]);
            client._status = RegistrationState::PassRegistered;
            // password assigned yazdır?;
            break;
        case RegistrationState::UsernameRegistered:
            client.setPassword(params[0]);
            // password changed yazdır?;
            break;
        }
    }
    if(client._channel && client._channel->_mode & Mode::KeyChannel)
        client._channel->setKey(params[0]);
}