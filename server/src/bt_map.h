#ifndef INCLUDED_BT_MAP
#define INCLUDED_BT_MAP

#include <iostream>
#include <vector>
#include <bt_model.h>


namespace bt {

/**
 * @brief: Level Map in square shapre
 */
class Map {
public:

    struct Cell {

        Cell(char type, int row, int col) {
            d_type = type;
            d_row = row;
            d_col = col;
        }

        char d_type;
        int d_row;
        int d_col;

        bool blockOnGround() const {
            if (d_type == '.') {
                return false;
            }
            else {
                return true;
            }
        }
    };
    // map width and height
    static const int CELL_SIZE = 16;
    static const int GRID_SIZE = 26;

    /**
     * @brief: Get the state of the map at the specific position
     * @param[in]: row Vertical position
     * @param[in]: col Horizontal position
     * @return: char value which describe the state of the terrain 
     */
    char state(int row, int col) const; 

    /**
     * @brief: Get the modifiable state of the map at the specific position
     * @param[in]: row Vertical position
     * @param[in]: col Horizontal position
     * @return: char reference to the value which describe the state of the terrain 
     */
    char& state(int row, int col);

    /**
     * @brief: Load the map from input stream which should contains SIZExSIZE
     * chars
     * @param[in]: is Input stream
     */
    void load(std::istream& is);

    /**
     * @brief: return true if the given area is free of obstacles
     * @param x
     * @param y
     * @param width
     * @param height
     * @return
     */
    bool isFree(double x, double y, double width, double height) const;


private:
    // state storage
    char d_state[GRID_SIZE][GRID_SIZE];

    std::vector<Cell> getOccupiedCells(double x, double y, double width, double height) const;
}; 

inline char Map::state(int row, int col) const {
    return d_state[row][col]; 
}

inline char& Map::state(int row, int col) {
    return d_state[row][col]; 
}

}
#endif
