#include <bt_level.h>

namespace bt {

Level::Level(std::vector<Player>& players) : 
    d_players(players), d_process(*this) {}


}
