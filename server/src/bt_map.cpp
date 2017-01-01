#include <bt_map.h>

namespace bt {

void Map::load(std::istream& is) {
    char c; 
    int row = 0, col = 0; 
    while (is >> c) {
        state(row, col) = c; 
        col += 1;  
        if (col == SIZE) {
            // move to next row
            col = 0; 
            row += 1; 
        }
    }

    std::cout << "Loaded map with total number of grids " << ((row - 1) * SIZE + col) << std::endl; 
}


}
