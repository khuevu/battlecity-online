#include <iostream>
#include <string>
#include <bt_server.h>

int main() {
    using namespace bt; 
    Server server(9034); 
    server.init(); 
    server.waitForPlayersToJoin(); 
    return 0;
}
