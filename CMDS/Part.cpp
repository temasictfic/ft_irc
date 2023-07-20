#include "Server.hpp"
#include "Channel.hpp"
#include "Replies.hpp"

//ERR_NEEDMOREPARAMS (461)
//ERR_NOSUCHCHANNEL (403)
//ERR_NOTONCHANNEL (442)

void Server::Part(Client &client, std::vector<const std::string &> params)
{
    // print params[1] as reason
    if(ParamsSizeControl(params,1))
     {
        sendServerToClient(client, ERR_NEEDMOREPARAMS(std::string("/PART")));
        return ;
     }
    _channels[params[0]].removeMembers(client);
}
