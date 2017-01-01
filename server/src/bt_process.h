#ifndef INCLUDED_BT_PROCESS
#define INCLUDED_BT_PROCESS

namespace bt {

class GameContainer; 

/**
 * @brief: Process message data from Players
 */
class Process {

public: 

    /**
     * @brief: Process Constructor which takes reference to Player 
     */
    Process(GameContainer& level); 

    /**
     * @brief: Process update which is invoked for every frame
     */
    bool loop(); 

private: 

    GameContainer& d_container;

    void process(int playerId, unsigned char msgId, const char* msg); 

}; 

}

#endif
