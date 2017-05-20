#include <bt_model.h>

namespace bt {

Model::Model(double x, double y,
             double width, double height,
             double speed, Direction d)
        : d_x(x), d_y(y),
          d_width(width),
          d_height(height),
          d_speed(speed),
          d_direction(d), d_status(READY) {}

} // close namespace
