#include <bt_map.h>

namespace bt {

void Map::load(std::istream& is) {
    char c; 
    int row = 0, col = 0; 
    while (is >> c) {
        state(row, col) = c; 
        col += 1;  
        if (col == GRID_SIZE) {
            // move to next row
            col = 0; 
            row += 1; 
        }
    }

    std::cout << "Loaded map with total number of grids " << ((row - 1) * GRID_SIZE + col) << std::endl;
}


std::vector<Map::Cell> Map::getOccupiedCells(double x, double y, double width, double height) const {
    // Note1: Do not track bullet on server side. Use client side to resolve.

    std::vector<Map::Cell> cells;

    for (int i = (int) (x / CELL_SIZE); i * CELL_SIZE < x + width; ++i)
    {
        for (int j = (int) (y / CELL_SIZE); j * CELL_SIZE < y + height; j++)
        {
            cells.push_back(Cell(state(i, j), i, j));
        }
    }

    return cells;
}

bool Map::isFree(double x, double y, double width, double height) const {
    auto cells = getOccupiedCells(x, y, width, height);

    for (auto& cell : cells) {
        if (cell.blockOnGround()) {
            return false;
        }
    }

    return true;
}


}
