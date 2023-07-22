#include <iostream>
#include "../inc/Server.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) 
    {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return 1;
    }
    try
    {
        Server IrcServ(argv[1],argv[2]);
        IrcServ.Listen().Run();  
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
