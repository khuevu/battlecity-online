#ifndef INCLUDED_BT_TANK
#define INCLUDED_BT_TANK

#include <bt_model.h>
#include <bt_util.h>


namespace bt {

/**
 * @brief: Tank base class
 */
class Tank : public Model {

public: 
    enum Action {
        FIRE,
        EXPLODE,
        ITEM
    };

    Tank(int id, double x, double y,
         double width, double height,
         double speed, Direction d);

    int id() const {
        return d_id;
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
        //TODO: We might want to keep track of player action at the server-side
        // in order to do validation
    }

protected: 

    // tank id
    const int d_id;
    // whether tank stops or moves
    bool d_stopped;
    
    int d_health; 
    // tank's bullet power
    int d_power; 
    // tank's speed
    int d_speed; 
};


/**
 * @brief: Player controlled Tank
 */
class PlayerTank : public Tank {

public: 

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

    EnemyTank(int id, double x, double y,
              double speed, Direction d, const GameContainer& g);

    bool loop(Clock::Milliseconds elapsedTime);
    
private: 

    const GameContainer& d_game; 
    
    // return true if there is no block on the destination. 
    bool canAdvance(Position targetPosition) const;

    // advance the tank 
    void tryAdvance(Clock::Milliseconds elapsedTime);

    // use weapon
    void tryFire();

};

}
#endif
