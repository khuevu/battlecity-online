#ifndef INCLUDED_BT_LEVEL
#define INCLUDED_BT_LEVEL

#include <vector>

#include <bt_player.h>
#include <bt_process.h>
#include <bt_map.h>

namespace bt {

class Level {
public: 
    Level(std::vector<Player>& players); 

    // players
    std::vector<Player>& d_players; 
    // process
    Process d_process; 
    // bullet list
    // explosion list?
    // enemy tank list
    // player tank list
    // map 
    Map d_map; 
}; 
}
#endif
