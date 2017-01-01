#include <iostream>
#include <fstream>

#include <bt_message.h>
#include <bt_gamecontainer.h>

namespace bt {

const std::string GameContainer::MAP_RESOURCE_PATH = "levels/"; 

GameContainer::GameContainer(int levelNumber, std::vector<Player>& players) : 
    d_id(levelNumber), d_players(players), 
    d_map(),
    d_process(*this),  
    d_state(NEW) {}

bool GameContainer::loop() {
    // first send the map to all player
    if (d_state == NEW) {
        sendMap();   
        d_state = WAITING; // wait for player to be ready before starting the game
        return true; 
    }
    // once all player ack starts running the game
    if (d_state == READY) {
        // send notification of ready game to players
        send(MsgTypePlayerReady, NULL, 0);         
        d_state = RUNNING; 
        return true; 
    }
    
    // game cycle
    if (d_state == RUNNING) {
        // loop other game objects  
        return true; 
    }

    // end game
    if (d_state == OVER) {
        return false; 
    }
}

void GameContainer::send(unsigned char msgId, const char* msg, size_t msgLength, int except) {
    for (Player player : d_players) {
        if (player.id() != except) {
            player.prepareMsgSend(msgId, msg, msgLength); 
        }
    }
}

void GameContainer::loadMap() {
    // read level map file
    std::ifstream in(MAP_RESOURCE_PATH + std::to_string(d_id));  
    // set the map terrains
    d_map.load(in);  
}

void GameContainer::sendMap() {
    MsgMapData mapDataMsg;
    for (int row = 0; row != Map::SIZE; ++row) {
        for (int col = 0; col != Map::SIZE; ++col) {
            mapDataMsg.map[row][col] = d_map.state(row, col); 
        }
    }

    send(MsgTypeMapData, (char *)&mapDataMsg, sizeof(mapDataMsg)); 
}

}
