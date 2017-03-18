#ifndef INCLUDED_BT_PLAYER
#define INCLUDED_BT_PLAYER

#include <cstdlib>

#include <bt_socket.h>

namespace bt {

class Player {
public: 
    enum OpStatus {
        SUCCESS,
        NODATA,
        FAILURE
    }; 

    /**
     * @brief: The size of the buffer that allocated per player
     */
    static const size_t BUFFER_SIZE = 1024;

    /**
     * @brief: Construct player that is connecting at the specified socket file
     * descriptor
     */
    Player(int socketFd, int playPosition); 

    /**
     * @brief: Get the playerId
     */
    int id() const {
        return d_socketFd; 
    }

    /**
     * @brief: Receive message from the client player and save to buffer
     */
    OpStatus receiveMsg(); 

    /**
     * @brief: Send message to the client player
     */
    OpStatus sendMsg(unsigned char msgId, const char *msg, size_t msgLength);

    /**
     * @brief: Get message from the buffer
     */
    OpStatus readNextMsgReceived(unsigned char* msgId, char* msg); 

    /**
     * @brief: Get the position the player play as
     */
    int position() const {
        return d_position;
    }

private: 
    // storage buffer
    char d_readBuffer[BUFFER_SIZE];
    char d_writeBuffer[BUFFER_SIZE];
    // buffer length
    size_t d_rbUsed; 
    // socket fd
    int d_socketFd; 
    // Socket handler
    ConnectSocket d_socket; 
    // Position of the player: first or second
    int d_position; 
}; 

    
}


#endif
