#ifndef INCLUDED_BT_MODEL
#define INCLUDED_BT_MODEL

#include <utility>
#include <bt_util.h>


namespace bt {

class Model {

public: 

    typedef std::pair<double, double> Position;

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

    Model(double x, double y,
          double width, double height,
          double speed, Direction d = UP);

    double x() const {
        return d_x; 
    }

    double y() const {
        return d_y; 
    }

    double width() const {
        return d_width;
    }

    double height() const {
        return d_height;
    }

    Direction direction() const {
        return d_direction;
    }

    Position getNextPosition(Clock::Milliseconds travelTime) const {
        double nextX = d_x;
        double nextY = d_y;

        if (d_direction == UP) {
            nextY = d_y - d_speed * travelTime;
        }
        else if (d_direction == LEFT) {
            nextX = d_x - d_speed * travelTime;
        }
        else if (d_direction == DOWN) {
            nextY = d_y + d_speed * travelTime;
        }
        else if (d_direction == RIGHT) {
            nextX = d_x + d_speed * travelTime;
        }

        return std::pair<double, double>(nextX, nextY);
    }

    bool overlap(const Model& other) const {
        double x1 = d_x;
        double x2 = d_x + d_width;
        double y1 = d_y;
        double y2 = d_y + d_height;

        double ox1 = other.x();
        double ox2 = ox1 + other.width();
        double oy1 = other.y();
        double oy2 = oy1 + other.height();

        // if outside
        if ((x2 <= ox1 || x1 >= ox2) ||
                (y1 >= oy2 || y2 <= oy1)) {
            return false;
        }
        else {
            return true;
        }
    }

private: 

protected: 

    // topleft corner position (x, y)
    double d_x; 
    double d_y;
    double d_width;
    double d_height;
    double d_speed;
    Direction d_direction;
    Status d_status;

};
} // close model namespace
#endif
