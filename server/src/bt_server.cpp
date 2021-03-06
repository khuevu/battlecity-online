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

#include <set>
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
            // construct a player at the current socket and play at the given
            // position
            int playPosition = d_players.size() + 1;
            d_players.emplace_back(newfd, playPosition); 
            // need to update the player abt their playing position
            MsgConfig config;
            config.position = (unsigned char) playPosition;
            std::cout << "Send config to player at position " << playPosition << std::endl;
            d_players.back().sendMsg(MsgTypeConfig, (char *) &config, sizeof(config));
        }
    }
    std::cout << "Two player joined. Notify the player that the game is ready" << std::endl;
    // send update to players
    for (Player& player : d_players) {
        player.sendMsg(MsgTypeGameReady, NULL, 0);
    }
}

void Server::waitForPlayersReadyForNewLevel() {
    std::set<int> readys; 
    while (readys.size() < d_players.size()) {
        // get the start level request
        for (Player& player : d_players) {

            player.receiveMsg();  
            unsigned char msgId; 

            Player::OpStatus rs = player.readNextMsgReceived(&msgId, NULL); 
            if (rs == Player::SUCCESS && msgId == MsgTypeRequestLevelStart) {
                std::cout << "Player " << player.id() << " is ready for new level" << std::endl;
                readys.insert(player.id());
            }
        }
    }
    std::cout << "Both players are ready for new level" << std::endl; 
}

void Server::receiveDataFromPlayers() {
    for (Player& player : d_players) {
        player.receiveMsg(); 
    }
}


void Server::startGame() {
    int level(1); 

    while (true) {
        std::cout << "Starting new level: level " << level << std::endl;
        // Wait for request to start level
        waitForPlayersReadyForNewLevel();
        // run next level
        bool playerWon = runLevel(level);

        if (playerWon) {
            std::cout << "Player won. Loading next level" << std::endl;
            level+= 1;
        }
        else {
            std::cout << "Player lost :(" << std::endl;
            break;
        }
    }
}

bool Server::runLevel(int levelNumber) {
    // initialize level container
    GameContainer container(levelNumber, d_players); 
    // start game loop

    while (container.loop()) {
        // read data from client sockets and store in player buffer to be
        // processed by the game logic
        receiveDataFromPlayers();
    }

    return container.playerWin();
}

}
