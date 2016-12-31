#ifndef INCLUDED_BT_MAP
#define INCLUDED_BT_MAP

namespace bt {

/**
 * @brief: Level Map in square shapre
 */
class Map {
public:
    // map width and height
    const static int SIZE = 26; 

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
     
private:
    // state storage
    char d_state[SIZE][SIZE];

}; 

inline char Map::state(int row, int col) const {
    return d_state[row][col]; 
}

inline char& Map::state(int row, int col) {
    return d_state[row][col]; 
}

}
#endif
