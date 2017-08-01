#ifndef INCLUDED_BT_TANK
#define INCLUDED_BT_TANK

#include <bt_model.h>
#include <bt_util.h>


namespace bt {

struct TankStat {
    TankStat(int type, double speed, double health, double power);
    const int type;
    const double speed;
    const double health;
    const double power;
};

//static const TankStat ENEMY_BASIC(1, 0.08, 100, 100);
//static const TankStat ENEMY_FAST(2, 0.13, 100, 100);
//static const TankStat ENEMY_POWER(3, 0.08, 200, 100);
//static const TankStat ENEMY_ARMOR(4, 0.08, 100, 400);


/**
 * @brief: Tank base class
 */
class Tank : public Model {

public:

    static const int TANK_SIZE = 26;

    enum Action {
        FIRE,
        EXPLODE,
        ITEM
    };

    enum Status {
        ACTIVE,
        FREEZED,
        DESTROYED
    };

    Tank(int id, double x, double y,
         TankStat stat, Direction d,
         double width = TANK_SIZE, double height = TANK_SIZE);

    int id() const {
        return d_id;
    }

    int type() const {
        return d_type;
    }

    int health() const {
        return d_health;
    }

    int power() const {
        return d_power;
    }

    void updateMovement(double fromX, double fromY, int direction, bool moving) {
        d_x = fromX; 
        d_y = fromY; 
        d_direction = static_cast<Direction>(direction);
        if (moving) {
            d_stopped = false;
        } else {
            d_stopped = true;
        }
    }

    void updateAction(Action action) {
        if (action == EXPLODE)
        {
            d_status = DESTROYED;
        }
    }

protected: 

    // tank id
    const int d_id;

    bool d_stopped;

    // tank type
    int d_type;

    int d_health;
    // tank's bullet power
    int d_power;

    Status d_status;
};


/**
 * @brief: Player controlled Tank
 */
class PlayerTank : public Tank {

public:
    static const TankStat STAT_BASIC;

    PlayerTank(int id, int playerId);

    int playerId() const {
        return d_playerId; 
    }

private: 

    int d_playerId; 

};


/**
 * @brief: AI controlled tank
 */
class GameContainer;

class EnemyTank : public Tank {

public:

    static const TankStat STAT_BASIC;
    static const TankStat STAT_FAST;
    static const TankStat STAT_POWER;
    static const TankStat STAT_ARMOR;
    static const int REQUIRED_RELOAD_TIME = 1000; // mislliseconds

    EnemyTank(int id, double x, double y,
              TankStat type, Direction d, GameContainer& g);

    bool loop(Clock::Milliseconds elapsedTime);
    
private: 

    GameContainer& d_game;
    
    // return true if there is no block on the destination. 
    bool canAdvance(Position targetPosition);

    // advance the tank 
    void tryAdvance(Clock::Milliseconds elapsedTime);

    // use weapon
    void tryFire(Clock::Milliseconds elapsedTime);

    Clock::Milliseconds d_reloadTime;
};

}
#endif
