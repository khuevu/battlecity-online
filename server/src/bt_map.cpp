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

Map::Cell::Cell(char type, int row, int col): d_type(type), d_row(row), d_col(col) {}

std::vector<Map::Cell> Map::getOccupiedCells(double x, double y, double width, double height) const {
    // Note1: Do not track bullet on server side. Use client side to resolve.

    std::vector<Map::Cell> cells;

    for (int i = (int) (y / CELL_SIZE); i * CELL_SIZE < y + height; ++i)
    {
        for (int j = (int) (x / CELL_SIZE); j * CELL_SIZE < x + width; j++)
        {
            cells.push_back(Cell(state(i, j), i, j));
        }
    }

    return cells;
}

bool Map::isFree(double x, double y, double width, double height) const {
//    std::cout << "Checking occupation of RECT [" << x << " - " << y << ", " << width << " " << height << "]" << std::endl;
    auto cells = getOccupiedCells(x, y, width, height);

    for (auto& cell : cells) {
//        std::cout << "Occupy cell " << cell.row() << " - " << cell.col() << std::endl;
        if (cell.blockOnGround()) {
//            std::cout << "Blocked" << std::endl;
            return false;
        }
    }

    return true;
}


}
