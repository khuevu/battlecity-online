#ifndef INCLUDED_BT_MODEL
#define INCLUDED_BT_MODEL

namespace bt {

class Model {

public: 

    enum Status {
        READY, 
        ACTIVE, 
        DESTROYED
    };

    enum Direction {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };

    Model(double x, double y, Direction d = UP);

    double x() const {
        return d_x; 
    }

    double y() const {
        return d_y; 
    }

    Direction direction() const {
        return d_direction;
    }

private: 

protected: 

    double d_x; 
    double d_y;
    Direction d_direction;
    Status d_status;

};
} // close model namespace
#endif
