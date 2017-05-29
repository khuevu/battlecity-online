#include <bt_tank.h>
#include <bt_gamecontainer.h>
#include <bt_util.h>
#include <set>


namespace bt {

namespace {

    Model::Direction chooseRandomDirection(Model::Direction except) {
        static std::vector<Model::Direction> directions = {Model::UP, Model::DOWN, Model::LEFT, Model::RIGHT};
        Model::Direction selected = except;
        while (selected == except) {
            selected = directions[randInt(0, directions.size())];
        }

        return selected;
    }

}

// TANK BASE CLASS ---------------------------------
Tank::Tank(int id, double x, double y,
           TankStat stat, Direction d,
           double width, double height) : Model(x, y, width, height, stat.speed, d), d_id(id), d_type(stat.type) {

}

TankStat::TankStat(int type, double s, double health, double power) :
        type(type), speed(s), health(health), power(power) {

}


// PLAYER TANK CLASS ---------------------------------

const TankStat PlayerTank::STAT_BASIC = TankStat(0, 0.08, 100, 100);

PlayerTank::PlayerTank(int id, int playerId) : 
    Tank(id, ((id == 1) ? 26 * 5 : 26 * 10), 300,
         STAT_BASIC, UP), d_playerId(playerId) {
}

// ENEMY TANK CLASS ---------------------------------

const TankStat EnemyTank::STAT_BASIC = TankStat(0, 0.08, 100, 100);
const TankStat EnemyTank::STAT_FAST = TankStat(1, 0.13, 100, 100);
const TankStat EnemyTank::STAT_POWER = TankStat(2, 0.08, 200, 100);
const TankStat EnemyTank::STAT_ARMOR = TankStat(3, 0.08, 100, 400);

EnemyTank::EnemyTank(int id, double x, double y,
                     TankStat stat, Direction d, GameContainer& g) :
    Tank(id, x, y, stat, d), d_game(g) {

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
    if (!d_game.d_map.isFree(targetPosition.first, targetPosition.second, d_width, d_height)) {
        return false;
    }
    // check if there are other tank obstacle
    for (auto& enemyTank : d_game.d_enemyTanks) {
        if (this->id() != enemyTank.id() && this->overlap(enemyTank)) {
            return false;
        }
    }

    for (auto &playerTank : d_game.d_playerTanks) {
        if (this->overlap(playerTank)) {
            return false;
        }
    }

    return true;
}

void EnemyTank::tryAdvance(Clock::Milliseconds elapsedTime) {
    // Get the next position in the current direction
    auto nextPosition = getNextPosition(elapsedTime);
    // check if is not blocked
    bool advanceable = canAdvance(nextPosition);
    // if not, advance
    if (advanceable) {
        // update tank position
        d_x = nextPosition.first;
        d_y = nextPosition.second;
    }
    // else if blocked, switch direction
    // 0.20 percent chance of switching direction
    if (!advanceable || random() < 0.2) {
        // change tank direction
        d_direction = chooseRandomDirection(d_direction);
    }

    std::cout << "Tank: " << this->id() << " can advance ? " << advanceable << std::endl;
    // send notification
    if (advanceable) {
        d_game.onEnemyTankAdvance(*this);
    }
}


void EnemyTank::tryFire() {
    // fire within a min-max delay range

}


//void EnemyTank::


}
