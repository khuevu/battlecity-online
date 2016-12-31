#ifndef INCLUDED_BT_PROCESS
#define INCLUDED_BT_PROCESS

namespace bt {

class Level; 

/**
 * @brief: Process message data from Players
 */
class Process {

public: 

    /**
     * @brief: Process Constructor which takes reference to Player 
     */
    Process(Level& level); 

    /**
     * @brief: Process update which is invoked for every frame
     */
    void update(); 

private: 

    Level& d_level;

    void process(int playerId, unsigned char msgId, const char* msg); 

}; 

}

#endif
