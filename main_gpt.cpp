#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>

const int MAX_CLIENTS = 10; // Maximum number of clients to handle
const int BUFFER_SIZE = 1024; // Size of the buffer for receiving client data

enum class ConnectionState {
    None,
    UserRegistration,
    NicknameRegistration,
    Registered
};

struct Client {
    int socket;
    std::string nickname;
    std::string username;
    bool isOperator;
    ConnectionState state;
};

std::vector<Client> clients; // Store connected clients

void processCommand(const std::string& command, const std::string& params, Client& client) {
    // Implement the logic to handle different IRC commands

    if (client.state == ConnectionState::None) {
        if (command == "NICK") {
            // Set client nickname
            client.nickname = params;
            client.state = ConnectionState::NicknameRegistration;
            std::cout << "Client " << client.socket << " set nickname: " << client.nickname << "\n";
        }
    }
    else if (client.state == ConnectionState::NicknameRegistration) {
        if (command == "USER") {
            // Set client username
            client.username = params;
            client.state = ConnectionState::UserRegistration;
            std::cout << "Client " << client.socket << " set username: " << client.username << "\n";

            // Send welcome message and confirm registration
            std::string welcomeMessage = "Welcome to the IRC server, " + client.nickname + "!";
            if (send(client.socket, welcomeMessage.c_str(), welcomeMessage.length(), 0) == -1) {
                std::cerr << "Failed to send welcome message to client.\n";
            }

            std::string registrationConfirm = "001 " + client.nickname + " :Welcome to the IRC server, " + client.nickname;
            if (send(client.socket, registrationConfirm.c_str(), registrationConfirm.length(), 0) == -1) {
                std::cerr << "Failed to send registration confirmation to client.\n";
            }

            client.state = ConnectionState::Registered;
        }
    }
    else if (client.state == ConnectionState::Registered) {
        // Handle other commands for registered clients
        // ...
    }
}

void processChatMessage(const std::string& message, Client& sender) {
    std::string formattedMessage = sender.nickname + ": " + message;

    for (const auto& client : clients) {
        if (client.socket != sender.socket) {
            if (send(client.socket, formattedMessage.c_str(), formattedMessage.length(), 0) == -1) {
                std::cerr << "Failed to send chat message to client.\n";
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return 1;
    }
    //

    int port = std::atoi(argv[1]);
    const char* password = argv[2];

    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    // Set socket options for reusability
    int reuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Failed to set socket options.\n";
        close(serverSocket);
        return 1;
    }

    // Bind socket to the specified port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket to port " << port << ".\n";
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Failed to listen on socket.\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "IRC server listening on port " << port << "...\n";

    while (true) {
        fd_set readSet;
        FD_ZERO(&readSet);

        int maxSocket = serverSocket;

        // Add server socket to the set
        FD_SET(serverSocket, &readSet);

        // Add client sockets to the set
        for (const auto& client : clients) {
            FD_SET(client.socket, &readSet);
            maxSocket = std::max(maxSocket, client.socket);
        }

        // Use select to wait for activity on sockets
        if (select(maxSocket + 1, &readSet, nullptr, nullptr, nullptr) == -1) {
            std::cerr << "Failed to select socket activity.\n";
            continue;
        }

        // Check if the server socket has activity (new client connection)
        if (FD_ISSET(serverSocket, &readSet)) {
            sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);

            // Accept a new connection
            int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);
            if (clientSocket == -1) {
                std::cerr << "Failed to accept client connection.\n";
                continue;
            }

            // Handle the client connection
            std::cout << "New client connected. Socket descriptor: " << clientSocket << "\n";

            // Set client socket to non-blocking mode
            int flags = fcntl(clientSocket, F_GETFL, 0);
            fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

            // Add client socket to the vector
            Client tmp;
            tmp.socket = clientSocket;
            tmp.nickname = "";
            tmp.username = "";
            tmp.isOperator = false;
            tmp.state = ConnectionState::None;
            clients.push_back(tmp); //{clientSocket, "", "", false, ConnectionState::None});
        }

        // Check client sockets for activity
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            int clientSocket = it->socket;

            if (FD_ISSET(clientSocket, &readSet)) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));

                // Read data from the client socket
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead == -1) {
                    std::cerr << "Failed to read from client socket.\n";
                    continue;
                }

                if (bytesRead == 0) {
                    // Connection closed by the client
                    std::cout << "Client disconnected. Socket descriptor: " << clientSocket << "\n";

                    // Remove client from the vector
                    it = clients.erase(it);
                    close(clientSocket);
                    continue;
                }

                // Process received data and handle IRC commands
                std::string message(buffer, bytesRead);
                std::cout << "Received data from client: " << message << "\n";

                // Check if the message starts with a command character
                if (message[0] == '/') {
                    // Extract command and parameters
                    size_t spacePos = message.find(' ');
                    std::string command = message.substr(1, spacePos - 1);
                    std::string params = message.substr(spacePos + 1);

                    // Process the IRC command
                    processCommand(command, params, *it);
                }
                else {
                    // Regular chat message
                    processChatMessage(message, *it);
                }
            }
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
