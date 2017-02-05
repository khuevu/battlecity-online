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
#include <bt_message.h>

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
    std::cout << "Two player joined. Notify the player that the game is ready" << std::endl;
    // send update to players
    for (Player& player : d_players) {
        player.prepareMsgSend(MsgTypeGameReady, NULL, 0); 
    }

    // flush
    sendDataToPlayers(); 
}

void Server::receiveDataFromPlayers() {
    for (Player& player : d_players) {
        player.receiveMsg(); 
    }
}

void Server::sendDataToPlayers() {
    for (Player& player : d_players) {
        player.sendMsg(); 
    }
}

void Server::startGame() {
    // TODO: wait for request to start level
    // run level
    runLevel(1); 
}

void Server::runLevel(int levelNumber) {
    // initialize level container
    GameContainer container(levelNumber, d_players); 
    // start game loop
    while (true) {
        // read data from client sockets and store in player buffer to be
        // processed by the game logic
        receiveDataFromPlayers(); 

        container.loop(); 

        // flush the data to be sent to players in the buffer to the client
        // sockets
        sendDataToPlayers();
    }
}

}
