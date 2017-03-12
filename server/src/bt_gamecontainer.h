#ifndef INCLUDED_BT_GAMECONTAINER
#define INCLUDED_BT_GAMECONTAINER

#include <vector>
#include <string>
#include <set>

#include <bt_player.h>
#include <bt_tank.h>
#include <bt_process.h>
#include <bt_map.h>
#include <bt_util.h>

namespace bt {

class GameContainer {
public: 
    // friends
    friend class Process; 

    const static std::string MAP_RESOURCE_PATH; 

    /**
     * @brief: GameContainer constructor 
     * @param[in]: levelNumber Level information to load map data
     * @param[in]: players Players connection data
     */
    GameContainer(int levelNumber, std::vector<Player>& players); 
    
    // run GameContainer 
    bool loop(); 

    
private: 
    // GameContainer id
    int d_id; 
    // players
    std::vector<Player>& d_players; 
    // bullet list
    // explosion list?
    // enemy tank list
    std::vector<EnemyTank> d_enemyTanks;
    // player tank list
    std::vector<PlayerTank> d_playerTanks;
    // map 
    Map d_map; 

    // process
    Process d_process; 
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
    Timer d_timer;
    unsigned long d_gameStartTime; 

    // ready state
    std::set<int> d_responses; 

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
    
    // send state of a given tank to players;
    //void sendTankStateUpdate(const Tank& target, int except=-1);
    void createPlayerTanks();

    // get the player with given id
    Player& getPlayer(int playerId) const;

    
}; 

}
#endif
