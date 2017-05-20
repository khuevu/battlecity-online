#include <bt_tank.h>

namespace bt {

namespace {
    const double TANK_SIZE = 26;
}

// TANK BASE CLASS ---------------------------------
Tank::Tank(int id, double x, double y,
           double width, double height,
           double speed, Direction d) : Model(x, y, width, height, speed, d), d_id(id) {

}


// PLAYER TANK CLASS ---------------------------------
PlayerTank::PlayerTank(int id, int playerId) : 
    Tank(id, ((id == 1) ? 26 * 5 : 26 * 10), 300,
         TANK_SIZE, TANK_SIZE,
         0.8, UP), d_playerId(playerId) {
}

// ENEMY TANK CLASS ---------------------------------
EnemyTank::EnemyTank(int id, double x, double y,
                     double speed, Direction d, const GameContainer& g) :
    Tank(id, x, y, TANK_SIZE, TANK_SIZE, speed, d), d_game(g) {

}

bool EnemyTank::loop(Clock::Milliseconds elapsedTime) {
    
    // make advance
    //
    tryAdvance(elapsedTime);
    
    // try shoot
    tryFire();

    return true;
}


bool EnemyTank::canAdvance(Position targetPosition) const {

    // check if there are obstacle on the map
    // check if there are other tank obstacle


    return true;
}

void EnemyTank::tryAdvance(Clock::Milliseconds elapsedTime) {
    // Get the next position in the current direction
    auto nextPosition = getNextPosition(elapsedTime);
    // check if is not blocked
    bool available = canAdvance(nextPosition);
    // if not, advance

    // else if blocked, switch direction
    // 0.20 percent chance of switching direction

    // send notification
}


void EnemyTank::tryFire() {
    // fire within a min-max delay range

}


//void EnemyTank::


}
