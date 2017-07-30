#ifndef INCLUDED_BT_GAMECONTAINER
#define INCLUDED_BT_GAMECONTAINER

#include <vector>
#include <string>
#include <set>

#include <bt_player.h>
#include <bt_model.h>
#include <bt_tank.h>
#include <bt_map.h>
#include <bt_util.h>
#include "bt_consentbox.h"

namespace bt {

class GameContainer {
public: 
    // friends
    friend class EnemyTank; 

    const static std::string MAP_RESOURCE_PATH; 

    /**
     * @brief: GameContainer constructor 
     * @param[in]: levelNumber Level information to load map data
     * @param[in]: players Players connection data
     */
    GameContainer(int levelNumber, std::vector<Player>& players);

    ~GameContainer();

    // run GameContainer 
    bool loop();

    
private: 
    // GameContainer id
    int d_id; 
    // players
    std::vector<Player>& d_players; 
    // enemy tank list
    std::vector<EnemyTank> d_enemyTanks;
    // player tank list
    std::vector<PlayerTank> d_playerTanks;
    // map 
    Map d_map; 

    // game state
    enum State {
        NEW,
        WAITING,
        READY,
        RUNNING,
        OVER
    }; 
    State d_state; 

    // utility timer
    Clock d_clock;
    unsigned long d_gameStartTime; 

    ConsentBox d_consentBox;

    char* d_msgBuffer;

    /**
     * @brief: Send messages to players. Messages are put in the buffer queue
     * to be flushed to the client
     * @param[in]: except The id of player except whom the messages will be
     */
    void send(unsigned char msgId, const char* msg, size_t msgLength, int except=-1); 

    /**
     * @brief: Read received messages from the players
     */
    void readMsgsFromPlayers(); 

    /**
     * @brief: Process message received from either of the player
     */
    void processMsg(int playerId, unsigned char msgId, const char* msg); 

    // load map from file
    void loadMap(); 
    // send map to players
    void sendMap();

//    const Map& map() const;
    
    // create PlayerTanks 
    void createPlayerTanks();

    // get the player with given id
    Player& getPlayer(int playerId) const;

    // handler when enemy tank fire
    void onEnemyTankFire(const Tank& tank);

    // handler when enemy tank advance
    void onEnemyTankAdvance(const Tank& tank);

    // Create new enemy tank and it those to the list
    void addNewEnemeyTank();
}; 


//inline const Map& GameContainer::map() const {
//    return d_map;
//}


}
#endif
