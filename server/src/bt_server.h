#ifndef INCLUDED_BATTLETANK_SERVER
#define INCLUDED_BATTLETANK_SERVER

#include <string>
#include <vector>
#include <bt_socket.h>
#include <bt_player.h>

namespace bt {
class Server {
public: 
    /**
     * @brief: Start game server at the specified port
     */ 
    Server(int port);

    /**
     * @brief: Wait for enough players to join the game
     */ 
    void waitForPlayersToJoin();

    /**
     * @brief: Start Game
     */
    void startGame(); 

    /**
     * @brief: play Level
     */
    void runLevel(int levelNumber); 

private:
    // cycle through the player sockets and receive data
    //void receiveDataFromPlayers(); 

    // socket to handle send and recv data
    ListenSocket d_listenSocket; 

    // number of supported players
    const size_t d_supported_players;  

    // connected players
    std::vector<Player> d_players; 
};

}

#endif
