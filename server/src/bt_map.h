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

    typedef std::pair<int, int> Cell;
    // map width and height
    static const int SIZE = 26; 

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
     
private:
    // state storage
    char d_state[SIZE][SIZE];

    std::vector<Cell> getOccupiedCells(const Model& model);

}; 

inline char Map::state(int row, int col) const {
    return d_state[row][col]; 
}

inline char& Map::state(int row, int col) {
    return d_state[row][col]; 
}

}
#endif
