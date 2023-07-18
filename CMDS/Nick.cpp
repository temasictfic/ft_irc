#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"

void Server::Nick(Client &client, std::vector<const std::string &> params)
{
    if (InvalidLetter(params[0]) || InvalidPrefix(params[0]))
        // hata durumu;
        ;
    else if (IsExistClient(params[0], 0))
        // hata durumu
        ;
    switch (client._status) // _status
    {
    case RegistrationState::PassRegistered:
        client._nick = ToLowercase(params[0]);
        client._status = RegistrationState::NickRegistered;
        // nick assigned yazdır?;
        break;
    default:
        client._nick = ToLowercase(params[0]);
        // nick changed yazdır?;
        break;
    }
}