#include <bt_server.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace bt {

Server::Server(int port) : d_numPlayer(0), d_socket(port) {
}

void Server::waitForPlayersToJoin() {
    while (d_numPlayer < 2) {
        int newfd = d_socket.receiveNewConnection(); 
        if (newfd != -1) {
            std::cout << "New player joined " << std::endl;  
            d_numPlayer += 1; 
        }
    }
    std::cout << "Two player joined" << std::endl;
}

}
