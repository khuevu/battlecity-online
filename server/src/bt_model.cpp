#include <bt_model.h>

namespace bt {

Model::Model(double x, double y, Direction d) : d_x(x), d_y(y), d_direction(d), d_status(READY) {}

} // close namespace
