#include "Server.hpp"

void Server::PrivMsg(class Client &client, std::vector<const std::string &> params)
{
    if (IsExistChannel(params[0]))
    {
        //_channels[NickName].getOperator()->sendmessage(Message);
    }
    else if (IsExistClient(params[0], 0))
    {
        // findClient(NickName).sendmessage(Message);
    }
}