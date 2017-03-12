#ifndef INCLUDED_BT_TANK
#define INCLUDED_BT_TANK

#include <bt_model.h>

namespace bt {

/**
 * @brief: Tank base class
 */
class Tank : public Model {

public: 
    enum Action {
        STOP,
        MOVE, 
        FIRE
    };

    Tank(int id, double x, double y, Direction d = UP); 

    int id() const {
        return d_id;
    }

    void update(double fromX, double fromY, int direction, int action) {
        d_x = fromX; 
        d_y = fromY; 
        d_direction = static_cast<Direction>(direction);

        //TODO: 
        if (action == STOP) {
            d_stopped = true;
        }
        else if (action == MOVE) {
            d_stopped = false;
        }
    }

private: 
    const int d_id;
    bool d_stopped;
};


/**
 * @brief: Player controlled Tank
 */
class PlayerTank : public Tank {

public: 

    PlayerTank(int id, int playerId);

    //virtual void move(double fromX, double fromY, int direction); 
    
    int playerId() const {
        return d_playerId; 
    }

private: 

    int d_playerId; 

};


/**
 * @brief: AI controlled tank
 */
class EnemyTank : public Tank {

public:

    EnemyTank(int id, double x, double y, Direction d = UP);

    //virtual void move(double fromX, double fromY, int direction); 

};

}
#endif
