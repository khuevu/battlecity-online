#ifndef INCLUDED_BATTLETANK_SERVER
#define INCLUDED_BATTLETANK_SERVER
#include <string>

namespace bt {
class Server {
public: 
    Server(int port);
    void init(); // start socket
    void waitForPlayersToJoin();
    //startGame(); 
    //BulletList d_bulletList; 
    //ExplosionList d_explosionList; 
    //PlayerList d_playerList; 
    //EnemyList d_enemyList; 
    //Map d_map; 

private:
    int d_port; 
    int d_socket;
    int d_numPlayer; 
      
};

}

#endif
