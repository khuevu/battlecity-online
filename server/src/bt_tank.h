#ifndef INCLUDED_BT_TANK
#define INCLUDED_BT_TANK

namespace bt {

/**
 * @brief: Tank base class
 */
class Tank {

public: 
    enum Status {
        READY, 
        ACTIVE, 
        DESTROYED
    };

    Tank(int id); 

    int id() const {
        return d_id;
    }

    double x() const {
        return d_x; 
    }

    double y() const {
        return d_y; 
    }

    int direction() const {
        return d_direction;
    }

    void updatePosition(double fromX, double fromY, int direction) {
        d_x = fromX; 
        d_y = fromY; 
        d_direction = direction;
    }

private: 
    const int d_id;
    Status d_status;
    double d_x; 
    double d_y;
    int d_direction;
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

    EnemyTank(int id);

    //virtual void move(double fromX, double fromY, int direction); 

};

}
#endif
