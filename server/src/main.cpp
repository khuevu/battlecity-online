#include <iostream>
#include <string>
#include <bt_server.h>

int main() {
    using namespace bt; 
    std::cout << "Starting BattleCity online server" << std::endl;
    Server server(9034); 

    std::cout << "Waiting for two players to join the game" << std::endl;
    server.waitForPlayersToJoin(); 
    
    std::cout << "Starting game " << std::endl; 
    // server.start game
    server.startGame(); 
}
