#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"

void Server::User(Client &client, std::vector<const std::string &> params)
{
    if (InvalidLetter(params[0]) || InvalidPrefix(params[0]) || InvalidLetter(params[1]) || InvalidPrefix(params[1]))
        // hata durumu;
        ;
    switch (client._status) // _status
    {
    case RegistrationState::NickRegistered:
        client._username = ToLowercase(params[0]);
        if(params.size() > 1)
            client._realname = params[1];
        client._status = RegistrationState::UsernameRegistered;
        // username assigned yazdır?;
        break;
    case RegistrationState::UsernameRegistered:
        client._username = ToLowercase(params[0]);
        if(params.size() > 1)
            client._realname = params[1];
        // username and realname changed yazdır?;
        break;
    }
}