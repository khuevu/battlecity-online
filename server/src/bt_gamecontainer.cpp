#include <iostream>
#include <fstream>

#include <bt_message.h>
#include <bt_gamecontainer.h>
#include <bt_tank.h>

namespace bt {

const std::string GameContainer::MAP_RESOURCE_PATH = "levels/"; 

GameContainer::GameContainer(int levelNumber, std::vector<Player>& players) : 
    d_id(levelNumber), d_players(players), 
    d_map(),
    d_process(*this),  
    d_state(NEW) {
        // load map data
        loadMap();  
    }

bool GameContainer::loop() {
    // first send the map to all player
    if (d_state == NEW) {
        std::cout << "Start level " << d_id << ". Sending map data to clients" << std::endl;
        sendMap();   
        // send creation of player tanks
        d_state = WAITING; // wait for player to be ready before starting the game
        // get timer going to to time the waiting time
        d_timer.start(); 
    }

    if (d_state == WAITING) {
        // new tank constructed when player acked receipt of map 
        if (d_responses.size() == 2) {
            std::cout << "All players has received map data and ready to start level" << std::endl; 
            // when the player are all ready
            // get the estimated time to communicate to both player (ms)
            unsigned long comTime = d_timer.stop(); 
            // level start time
            d_gameStartTime = comTime * 2 + currentTimeInMilliseconds(); 
            // set game to ready
            d_state = READY;
            // send the game expected start time to client
            MsgLevelStart msg; 
            msg.startTime = d_gameStartTime; 
            std::cout << "Level should starts at " << d_gameStartTime << std::endl; 
            send(MsgTypeLevelStart, (char*) &msg, sizeof(msg));  
            // clear responses tracker
            d_responses.clear();
        } 
    }

    if (d_state == READY) {
        if (currentTimeInMilliseconds() >= d_gameStartTime) {
            std::cout << "Game starts" << std::endl;
            d_state = RUNNING; 
            // create and send information about creating player tank
            createPlayerTanks();
        }
    }
    
    // game cycle
    if (d_state == RUNNING) {
        // loop other game objects  
    }

    // end game
    if (d_state == OVER) {
        return false; 
    }

    readMsgsFromPlayers();  
    return true; 
}

void GameContainer::send(unsigned char msgId, const char* msg, size_t msgLength, int except) {
    for (Player& player : d_players) {
        if (player.id() != except) {
            player.prepareMsgSend(msgId, msg, msgLength); 
        }
    }
}

void GameContainer::readMsgsFromPlayers() {
    for (Player& player : d_players) {
        unsigned char msgId; 
        // use a 1024 buffer at most
        char msg[1024]; 

        Player::OpStatus rc = player.readNextMsgReceived(&msgId, msg); 
        if (rc == Player::SUCCESS) {
            processMsg(player.id(), msgId, msg);  
        }
    }
}


void GameContainer::processMsg(int playerId, unsigned char msgId, const char* msg) {
    std::cout << "Process msg " << msgId << " from player " << playerId << std::endl; 

    // when player acknowledge that it has received map data
    if (msgId == MsgTypeLevelReady) {
        std::cout << "Player " << playerId << " received map" << std::endl;
        // create new PlayerTank to join the game 
        d_responses.emplace(playerId);
    }

    if (msgId == MsgTypeTankUpdate) {
        const MsgTankUpdate* msgUpdate = (const MsgTankUpdate*) msg;
        std::cout << "Tank " << msgUpdate->tankId << " change direction " << std::endl;
        // perform the move
        for (PlayerTank& tank : d_playerTanks) {
            if (tank.id() == msgUpdate->tankId) {
                tank.update(msgUpdate->x, msgUpdate->y, msgUpdate->direction, msgUpdate->action);
                // send update to the other player except the current player
                std::cout << "Send update to other player " << sizeof(*msgUpdate) << std::endl;
                send(MsgTypeTankUpdate, (char*) msgUpdate, sizeof(*msgUpdate), tank.playerId()); 
            }
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
    std::cout << "Send map data of size " << sizeof(mapDataMsg) << std::endl;
    send(MsgTypeLevelMapData, (char *)&mapDataMsg, sizeof(mapDataMsg)); 
}

//void GameContainer::sendTankStateUpdate(const Tank& target, int except) {
    //// construct message
    //MsgTankMove msgTankMove; 
    //msgTankMove.tankId = target.id(); 
    //msgTankMove.x = target.x(); 
    //msgTankMove.y = target.y(); 
    //msgTankMove.direction = target.direction(); 
    //msgTankMove.action = target.

    //send(MsgTypeTankMove, (char*) &msgTankMove, sizeof(msgTankMove), except); 
//}

void GameContainer::createPlayerTanks()
{
    std::cout << "Spawn players tank" << std::endl;
    for (Player& player : d_players) {
        d_playerTanks.emplace_back(player.position(), player.id());                          
        // send update to client
        MsgTankCreation msgTankCreate;
        msgTankCreate.tankId = player.position(); 
        msgTankCreate.direction = d_playerTanks.back().direction();
        msgTankCreate.x = d_playerTanks.back().x(); 
        msgTankCreate.y = d_playerTanks.back().y();
        send(MsgTypeTankCreation, (char*) &msgTankCreate, sizeof(msgTankCreate));
    }
}

Player& GameContainer::getPlayer(int playerId) const {
    for (Player& player : d_players) {
        if (player.id() == playerId) {
            return player;
        }
    }
    
    throw std::runtime_error("Invalid player Id ");
}

}
