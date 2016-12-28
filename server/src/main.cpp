#include <iostream>
#include <string>
#include <bt_server.h>

int main() {
    using namespace bt; 
    Server server(9034); 
    server.waitForPlayersToJoin(); 
    // server.start game
    // LOOP
    // server.receive Player data -> save to player buffer
    // game.process -> process player data buffer
    // game.object -> update
    return 0;
}
