#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

Server::Server(const std::string &Port, const std::string &Password)
{
    if (Port.empty())
    {
        _port = 6667;
        std::cout << "Port is not given so your server initialized with default port 6667\n";
    }
    else
    {
        _port = std::atoi(Port.c_str());
        if (_port < 1024 || _port > 49151)
            throw InvalidPortException();
    }
    if (Password.empty())
    {
    }
    else
        _password = Password;

     if(ReadySocketandPort())
        return ;


}

void Server::Detect(fd_set readSet)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            int clientSocket = it->getSocketFd();

            if (FD_ISSET(clientSocket, &readSet))
            {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));

                // Read data from the client socket
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead == -1)
                {
                    std::cerr << "Failed to read from client socket.\n";
                    continue;
                }

                if (bytesRead == 0)
                {
                    // Connection closed by the client
                    std::cout << "Client disconnected. Socket descriptor: " << clientSocket << "\n";

                    // Remove client from the vector
                    it = _clients.erase(it);
                    close(clientSocket);
                    continue;
                }
//check message length < 512 including /r/n
                // Process received data and handle IRC commands
                std::string message(buffer, bytesRead);
                std::cout << "Received data from client: " << message << "\n";
//Serve::parse
                // Check if the message starts with a command character
                if (message[0] == '/')
                    ParseAndExecute(message,*it);
                else
                    ;//sendMessageToChannel(it->getSocketFd(), it->_channel->_name, message);
            }
        }
}

void Server::ParseAndExecute(const std::string &message, Client &client)
{
    size_t spacePos = message.find(' ');
    std::string command = message.substr(1, spacePos - 1);
    if (cmds.find(command) != cmds.end())
			(*cmds.at(ToUppercase(command)))(client,  split(message.substr(spacePos + 1)," "));
    else
    {
        // hata yazdır
    }
    
}

void Server::Run()
{
    while (true)
    {
        fd_set readSet;
        FD_ZERO(&readSet);

        int maxSocket = _serverSocketFd;

        // Add server socket to the set
        FD_SET(_serverSocketFd, &readSet);

        // Add client sockets to the set
        for (std::vector<Client>::iterator client = _clients.begin(); client != _clients.end(); client++)
        {
            FD_SET(client->getSocketFd(), &readSet);
            maxSocket = std::max(maxSocket, client->getSocketFd());
        }

        // Use select to wait for activity on sockets
        if (select(maxSocket + 1, &readSet, nullptr, nullptr, nullptr) == -1)
        {
            std::cerr << "Failed to select socket activity.\n";
            continue;
        }

        // Check if the server socket has activity (new client connection)
        if (FD_ISSET(_serverSocketFd, &readSet))
        {
            sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);

            // Accept a new connection
            int clientSocket = accept(_serverSocketFd, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressLength);
            if (clientSocket == -1)
            {
                std::cerr << "Failed to accept client connection.\n";
                continue;
            }

            // Handle the client connection
            std::cout << "New client connected. Socket descriptor: " << clientSocket << "\n";

            // Set client socket to non-blocking mode
            int flags = fcntl(clientSocket, F_GETFL, 0);
            fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

            // Add client socket to the vector
            size_t idx = _clients.size();
            _clients.push_back(Client(clientSocket, idx));
        }

    // Server::serve()
        // Check client sockets for activity
        Detect(readSet);    
    }
}
bool Server::ReadySocketandPort()
{
    _serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocketFd == -1)
    {
        std::cerr << "Failed to create socket.\n";
        return true;
    }

    int reuse = 1;
    if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        std::cerr << "Failed to set socket options.\n";
        close(_serverSocketFd);
        return true;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(_serverSocketFd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind socket to port " << _port << ".\n";
        close(_serverSocketFd);
        return true;
    }

    // Listen for incoming connections
    if (listen(_serverSocketFd, MAX_CLIENTS) == -1)
    {
        std::cerr << "Failed to listen on socket.\n";
        close(_serverSocketFd);
        return true;
    }

    std::cout << "IRC server listening on port " << _port << "...\n";
}


int Server::sendServerToClient(Client& reciever, const std::string &message)
{
    if (send(reciever.getSocketFd(), message.c_str(), message.length(), 0) == -1)
    {
        std::cerr << "Failed to send chat message between " << "ircserv" << " -> " << reciever._nick << "\n";
        return -1;
    }
    return 0;
}

int Server::sendClientToClient(Client& sender, Client& reciever, const std::string &message)
{
    std::string formattedMessage = sender._nick + ": " + message;
    if (send(reciever.getSocketFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
    {
        std::cerr << "Failed to send chat message between " << sender._nick << " -> " << reciever._nick << "\n";
        return -1;
    }
    return 0;
}
int Server::sendClientToChannel(Client& sender, const std::string& ChannelName, const std::string &message)
{
    if(!sender._channel)
        return 0;
    std::string formattedMessage = sender._nick + ": " + message;
    std::vector<Client>::iterator client = _channels.at(ChannelName).getMembers().begin();
    std::vector<Client>::iterator end = _channels.at(ChannelName).getMembers().end();
    for (; client != end; client++)
    {
        if (client->getSocketFd() != sender.getSocketFd()) 
        {
            if (send(client->getSocketFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
            {
                std::cerr << "Failed to send chat message between " << sender._nick << " -> " << client->_nick << "\n";
                continue;
            }
        }
    }
    return 1;
}
