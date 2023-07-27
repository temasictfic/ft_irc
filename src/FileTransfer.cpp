#include "../inc/Server.hpp"

int fileTransfer(const std::string& command, Client& commander, const std::string& receiverName, const std::string& filename)
{
    if (command == "send")
    {
        if (IsExistClient(receiverName))
        {
            Client& receiver = findClient(receiverName);
            sendClientToClient(commander, receiver, commander._nick + " wants to send you this file: " + filename + " . If you want to accept '!FILE y' or '!FILE n'.");
            receiver._fileinfos.push_back(commander._nick);
            receiver._fileinfos.push_back(filename);
        }
        else
            sendServerToClient(commander, ERR_NOSUCHNICK(receiverName));
    }
    else if (command == "y")
    {
        if (commander._fileinfos.empty())
            return -1;
        std::string fileName = commander._fileinfos[1];

        Client& accepted = findClient(commander._fileinfos[0]);
        sendServerToClient(accepted._nick, "File Transfer Accepted.");
        commander._fileinfos.clear();
        //implement send file logic
        sendFileToClient(accepted, commander, filename); //?

        sendServerToClient(accepted, fileName + " is sent."); // Bu ikisi sendFileToClient içinde olabilir.
        sendServerToClient(commander, fileName + " received.");
    }
    else if (command == "n")
    {
        if (commander._fileinfos.empty())
            return -1;
        Client& rejected = findClient(commander._fileinfos[0]);
        sendServerToClient(rejected._nick, "File Transfer Rejected.");
        commander._fileinfos.clear();
    }
}