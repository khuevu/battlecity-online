#ifndef INCLUDED_BT_GAMECONTAINER
#define INCLUDED_BT_GAMECONTAINER

#include <vector>
#include <string>

#include <bt_player.h>
#include <bt_process.h>
#include <bt_map.h>

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

    /**
     * @brief: Send messages to players
     * @param[in]: except The id of player except whom the messages will be
     */
    void send(unsigned char msgId, const char* msg, size_t msgLength, int except=-1); 

private: 
    // GameContainer id
    int d_id; 
    // players
    std::vector<Player>& d_players; 
    // bullet list
    // explosion list?
    // enemy tank list
    // player tank list
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
    // load map from file
    void loadMap(); 
    // send map to players
    void sendMap(); 
    
}; 

}
#endif
