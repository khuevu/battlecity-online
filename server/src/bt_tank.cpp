#include <bt_tank.h>

namespace bt {

// TANK BASE CLASS ---------------------------------
Tank::Tank(int id, double x, double y, Direction d) : Model(x, y, d), d_id(id) {

}


// PLAYER TANK CLASS ---------------------------------
PlayerTank::PlayerTank(int id, int playerId) : 
    Tank(id, ((id == 1) ? 26 * 5 : 26 * 10), 300, UP), d_playerId(playerId) {
}

//void PlayerTank::move(double fromX, double fromY, int direction) {
    //// update the postion
    //d_x = fromX
    //d_y = fromY
    //// send the data to other player
//}
    

// ENEMY TANK CLASS ---------------------------------
EnemyTank::EnemyTank(int id, double x, double y, Direction d) : Tank(id, x, y, d) {

}


}
