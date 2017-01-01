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

#include <bt_server.h>
#include <bt_gamecontainer.h>

namespace bt {

Server::Server(int port) : d_listenSocket(port), d_supported_players(2) {
}

void Server::waitForPlayersToJoin() {
    while (d_players.size() < d_supported_players) {
        int newfd = d_listenSocket.receiveNewConnection(); 
        if (newfd != -1) {
            std::cout << "New player joined " << std::endl;  
            d_players.emplace_back(newfd); 
        }
    }
    std::cout << "Two player joined" << std::endl;
}

void Server::startGame() {
    // send level data to client
    // run level
}

void Server::runLevel(int levelNumber) {
    // initialize level container
    GameContainer container(levelNumber, d_players); 
    // start game loop
    //  receive Player msg
    //  update container
    //  send Player msg
}

}
