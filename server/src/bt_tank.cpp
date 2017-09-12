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
           double width, double height) :
        Model(x, y, width, height, stat.speed, d),
        d_id(id), d_type(stat.type),
        d_health(stat.health), d_power(stat.power) {


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

TankStat EnemyTank::tankStatFromType(int type) {
    static std::vector<TankStat> stats = {STAT_BASIC, STAT_FAST, STAT_POWER, STAT_ARMOR};
    for (const auto& stat : stats) {
        if (stat.type == type) {
            return stat;
        }
    }

    return STAT_BASIC;
}

EnemyTank::EnemyTank(int id, double x, double y,
                     TankStat stat, Direction d, GameContainer& g) :
    Tank(id, x, y, stat, d),
    d_game(g),
    d_reloadTime(0),
    d_spawnTime(0),
    d_spawning(true)
{

}


bool EnemyTank::loop(Clock::Milliseconds elapsedTime) {
    if (d_spawnTime < REQUIRED_SPAWN_TIME)
    {
        d_spawnTime += elapsedTime;
        return true;
    }
    else if (d_spawning)
    {
        // send update of tank movement for the first time after spawining
        d_game.onEnemyTankAdvance(*this);
        d_spawning = false;
    }

    if (d_status == DESTROYED)
    {
        return false;
    }
    // make advance
    //
    tryAdvance(elapsedTime);
    
    // try shoot
    tryFire(elapsedTime);

    return true;
}


bool EnemyTank::canAdvance(Position targetPosition) {

    // check if the tank is inside the map
    if (!d_game.d_map.isWithin(targetPosition.first, targetPosition.second, d_width, d_height)) {
        return false;
    }
    // check if there are obstacle on the map
    if (!d_game.d_map.isFree(targetPosition.first, targetPosition.second, d_width, d_height)) {
        return false;
    }
    // check if there are other tank obstacle
    for (auto& enemyTank : d_game.d_enemyTanks) {
        if (this->id() != enemyTank.id() && enemyTank.overlap(targetPosition.first, targetPosition.second, d_width, d_height)) {
            return false;
        }
    }

    for (auto &playerTank : d_game.d_playerTanks) {
        if (playerTank.overlap(targetPosition.first, targetPosition.second, d_width, d_height)) {
            return false;
        }
    }

    return true;
}

void EnemyTank::tryAdvance(Clock::Milliseconds elapsedTime) {
    // Get the next position in the current direction
    auto nextPosition = getNextPosition(elapsedTime);
    bool advanceable = canAdvance(nextPosition);
    // if not, advance
    if (advanceable) {
        // update tank position
        d_x = nextPosition.first;
        d_y = nextPosition.second;

    } else {
        std::cout << "Enemy Change tank direction " << std::endl;
        // change tank direction
        d_direction = chooseRandomDirection(d_direction);
        d_game.onEnemyTankAdvance(*this);

    }
    //TODO: Add randomness: chnage direction even if it is not blocked
}


void EnemyTank::tryFire(Clock::Milliseconds elapsedTime) {
    // fire within a min-max delay range
    d_reloadTime += elapsedTime;

    if (d_reloadTime >= REQUIRED_RELOAD_TIME) {
        // fire
        d_game.onEnemyTankFire(*this);
        d_reloadTime = 0;
    }
}


//void EnemyTank::


}
