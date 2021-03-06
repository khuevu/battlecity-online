#include <iostream>
#include <fstream>

#include <bt_message.h>
#include <bt_gamecontainer.h>
#include <bt_tank.h>
#include <bt_util.h>


namespace bt {

namespace {
    const int ENEMY_NUMBER = 10;
    const Clock::Milliseconds ENEMY_SPAWN_RECHARGE_TIME = 2 * 1000;
}

const std::string GameContainer::MAP_RESOURCE_PATH = "levels/"; 

GameContainer::GameContainer(int levelNumber, std::vector<Player>& players) : 
    d_id(levelNumber), d_players(players), 
    d_map(),
    d_state(NEW),
    d_msgBuffer(new char[1024]),
    d_timeSinceLastEnemyAdded(ENEMY_SPAWN_RECHARGE_TIME)
{
    // load map data
    loadMap();
}


GameContainer::~GameContainer()
{
    delete[] d_msgBuffer;
}

bool GameContainer::playerWin() const {
    return d_playerWin;
}

bool GameContainer::loop() {
    // first send the map to all player
    if (d_state == NEW) {
        std::cout << "Start level " << d_id << ". Sending map data to clients" << std::endl;
        sendMap();   
        // send creation of player tanks
        d_state = WAITING; // wait for player to be ready before starting the game
        // get timer going to to time the waiting time
        d_clock.reset();
    }


    if (d_state == READY) {
        if (currentTimeInMilliseconds() >= d_gameStartTime) {
            std::cout << "Game starts" << std::endl;
            d_state = RUNNING; 
            // create and send information about creating player tank
            createPlayerTanks();
            // reset clock tick to go to game main loop
            d_clock.reset();
        }
    }
    
    // game cycle
    if (d_state == RUNNING) {
        Clock::Milliseconds elapsedTime = d_clock.tick();
        // update states of current enemy tanks
        for (auto &enemy : d_enemyTanks) {
            enemy.loop(elapsedTime);
        }

        // create enemy tanks 
        addNewEnemeyTank(elapsedTime);
        //TODO: gen reward item
    }

    // end game
    if (d_state == OVER) {
        return false; 
    }

    readMsgsFromPlayers();
    processConsensuses();
    return true; 
}

void GameContainer::send(unsigned char msgId, const char* msg, size_t msgLength, int except) {
    for (Player& player : d_players) {
        if (player.id() != except) {
            player.sendMsg(msgId, msg, msgLength);
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
            processPlayerMsg(player.id(), msgId, msg);
        }
    }

}


void GameContainer::processConsensuses()
{
    // new tank constructed when player acked receipt of map
    //if (d_responses.size() == 2) {
    int msgId = d_consentBox.getNextConsensus(d_msgBuffer);

    if (msgId == MsgTypeLevelReady) {
        std::cout << "All players has received map data and ready to start level" << std::endl;
        // when the player are all ready
        // get the estimated time to communicate to both player (ms)
        //Clock::Milliseconds comTime = d_clock.tick();
        Clock::Milliseconds comTime = 2000;
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
        //d_responses.clear();
    }
    else if (msgId == MsgTypeTankAction)
    {
        const MsgTankAction* msgAction = (const MsgTankAction*) d_msgBuffer;
        // update tank action
        for (EnemyTank& tank : d_enemyTanks) {
            if (tank.id() == msgAction->tankId) {
                tank.updateAction(static_cast<Tank::Action >(msgAction->action));
            }
        }

        std::cout << "Sending a message from consent with msgId " << msgId << std::endl;
        // send update to client
        send(MsgTypeTankAction, (char*)msgAction, sizeof(*msgAction));
    }
    else if (msgId == MsgTypeGameEnd)
    {
        const MsgGameEnd* msgGameEnd = (const MsgGameEnd*) d_msgBuffer;
        // game over
        std::cout << "Game over. Player win ? " << msgGameEnd->playerWin << std::endl;
        // end game
        d_playerWin = msgGameEnd->playerWin;
        d_state = OVER;
    }
}


void GameContainer::processPlayerMsg(int playerId, unsigned char msgId, const char *msg) {
    std::cout << "Process msg " << msgId << " from player " << playerId << std::endl; 

    // when player acknowledge that it has received map data
    if (msgId == MsgTypeLevelReady) {
        std::cout << "Player " << playerId << " received map" << std::endl;
        d_consentBox.vote(playerId, MsgTypeLevelReady, NULL, 0);
    }

    else if (msgId == MsgTypeTankMovement) {
        const MsgTankMovement* msgMovement = (const MsgTankMovement*) msg;
        std::cout << "Tank " << msgMovement->tankId << " change direction " << std::endl;
        // perform the move
        for (PlayerTank& tank : d_playerTanks) {
            if (tank.id() == msgMovement->tankId) {
                tank.updateMovement(msgMovement->x, msgMovement->y, msgMovement->direction, msgMovement->moving);
                // send update to the other player except the current player
                send(MsgTypeTankMovement, (char*) msgMovement, sizeof(*msgMovement), tank.playerId());
            }
        }
    }

    else if (msgId == MsgTypeTankAction) {
        const MsgTankAction* msgAction = (const MsgTankAction*) msg;

        std::cout << "Tank " << msgAction->tankId << " action: " << msgAction->action << std::endl;
        // if action only concens player
        if (msgAction->tankId <= 2) {
            for (PlayerTank& tank : d_playerTanks) {
                if (tank.id() == msgAction->tankId) {
                    tank.updateAction(static_cast<Tank::Action >(msgAction->action));
                    send(MsgTypeTankAction, (char*) msgAction, sizeof(*msgAction), tank.playerId());
                }
            }
        }
        else {
            // if action concerns enemies, put it to consentbox
            std::cout << "Player " << playerId << " vote for action " << msgAction->action << " for tank "
                      << msgAction->tankId << std::endl;
            d_consentBox.vote(playerId, msgId, msg, sizeof(*msgAction));
        }
    }

    else if (msgId == MsgTypeGameEnd) {
        const MsgGameEnd* msgGameEnd = (const MsgGameEnd*) msg;
        std::cout << "Player " << playerId << " vote end game " << std::endl;
        d_consentBox.vote(playerId, msgId, msg, sizeof(*msgGameEnd));
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
    for (int row = 0; row != Map::GRID_SIZE; ++row) {
        for (int col = 0; col != Map::GRID_SIZE; ++col) {
            mapDataMsg.map[row][col] = d_map.state(row, col); 
        }
    }
    std::cout << "Send map data of size " << sizeof(mapDataMsg) << std::endl;
    send(MsgTypeLevelMapData, (char *)&mapDataMsg, sizeof(mapDataMsg)); 
}


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


void GameContainer::onEnemyTankFire(const Tank& tank) {
    MsgTankAction msgAction;
    msgAction.tankId = tank.id();
    msgAction.action = Tank::FIRE;
    send(MsgTypeTankAction, (char*) &msgAction, sizeof(msgAction));
    std::cout << "Sent msg to update enemy tank fire of tank " << tank.id() << std::endl;
}


void GameContainer::onEnemyTankAdvance(const Tank& tank) {
    MsgTankMovement msgTankMovement;
    msgTankMovement.tankId = tank.id();
    msgTankMovement.direction = tank.direction();
    // moving flag is not important for enemy tank as they will always move
    msgTankMovement.moving = true;
    msgTankMovement.x = tank.x();
    msgTankMovement.y = tank.y();
    send(MsgTypeTankMovement, (char *) &msgTankMovement, sizeof(msgTankMovement));
    std::cout << "Sent msg to update enemy tank movement of tank " << tank.id() << std::endl;
}

void GameContainer::addNewEnemeyTank(Clock::Milliseconds elapsedTime) {
    if (d_enemyTanks.size() == ENEMY_NUMBER)
    {
        // Do not generate new tank if the number of tanks meet the limit
        return;
    }

    // only add new enemy after recharge time
    d_timeSinceLastEnemyAdded += elapsedTime;
    if (d_timeSinceLastEnemyAdded < ENEMY_SPAWN_RECHARGE_TIME)
    {
        return;
    }

    int n = randInt(1, 2);
    n = (ENEMY_NUMBER - d_enemyTanks.size()) >= n ? n : 0;
    std::cout << "Generating new number of enemy " << n << std::endl;

    for (int i = 0; i != n; ++i) {
        // find the possible position of the tank
        double spawnX, spawnY;
        bool found = findSpawningPosition(&spawnX, &spawnY);
        if (!found)
        {
            // Skip this round of generate enemy tank if the position to generate the tank not found
            continue;
        }

        int enemyTankId = 3 + d_enemyTanks.size(); // offset
        // random the type of tank
        auto stat = EnemyTank::tankStatFromType(randInt(0, 3));
        d_enemyTanks.push_back(EnemyTank(enemyTankId, spawnX, spawnY, stat, Model::DOWN, *this));
        const EnemyTank& newTank = d_enemyTanks.back();
        // send tank generate event
        MsgTankCreation msgTankCreate;
        msgTankCreate.tankId = newTank.id();
        msgTankCreate.type = newTank.type();
        msgTankCreate.direction = newTank.direction();
        msgTankCreate.x = newTank.x();
        msgTankCreate.y = newTank.y();
        msgTankCreate.speed = newTank.speed();
        msgTankCreate.health = newTank.health();
        msgTankCreate.power = newTank.power();

        std::cout << "Sent msg to create new enemy tank with id " << newTank.id() << std::endl;
        send(MsgTypeTankCreation, (char*) &msgTankCreate, sizeof(msgTankCreate));

        //onEnemyTankAdvance(newTank);
    }

    d_timeSinceLastEnemyAdded = 0;
}


bool GameContainer::findSpawningPosition(double* x, double* y)
{
    // get free position on the map within the upper half of the map
    double possibleX = randInt(0, d_map.GRID_SIZE / 2) * d_map.CELL_SIZE * 2;
    double possibleY = randInt(0, d_map.GRID_SIZE / 4) * d_map.CELL_SIZE * 2;

    // check if the position is occupied
    if (!d_map.isWithin(possibleX, possibleY, EnemyTank::TANK_SIZE,  EnemyTank::TANK_SIZE)) {
        return false;
    }
    // check if there are obstacle on the map
    if (!d_map.isFree(possibleX, possibleY,  EnemyTank::TANK_SIZE,  EnemyTank::TANK_SIZE)) {
        return false;
    }

    // check if there are other tank obstacle
    for (auto& enemyTank : d_enemyTanks) {
        if (enemyTank.overlap(possibleX, possibleY, EnemyTank::TANK_SIZE, EnemyTank::TANK_SIZE)) {
            return false;
        }
    }

    for (auto &playerTank : d_playerTanks) {
        if (playerTank.overlap(possibleX, possibleY, EnemyTank::TANK_SIZE, EnemyTank::TANK_SIZE)) {
            return false;
        }
    }

    *x = possibleX;
    *y = possibleY;
    return true;
}





}
