#ifndef INCLUDED_BATTLETANK_SERVER
#define INCLUDED_BATTLETANK_SERVER
#include <string>
#include <bt_socket.h>

namespace bt {
class Server {
public: 
    Server(int port);
    void waitForPlayersToJoin();
    
private:
    //int d_port; 
    //int d_socket;
    int d_numPlayer; 
    Socket d_socket; 
      
};

}

#endif
