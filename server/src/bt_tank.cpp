#include <bt_tank.h>

namespace bt {

// TANK BASE CLASS ---------------------------------
Tank::Tank(int id) : d_id(id), d_status(READY) {

}


// PLAYER TANK CLASS ---------------------------------
PlayerTank::PlayerTank(int id, int playerId) : Tank(id), d_playerId(playerId) {

}

//void PlayerTank::move(double fromX, double fromY, int direction) {
    //// update the postion
    //d_x = fromX
    //d_y = fromY
    //// send the data to other player
//}
    

// ENEMY TANK CLASS ---------------------------------
EnemyTank::EnemyTank(int id) : Tank(id) {

}


}
