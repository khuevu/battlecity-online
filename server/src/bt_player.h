#ifndef INCLUDED_BT_PLAYER
#define INCLUDED_BT_PLAYER

#include <cstdlib>

#include <bt_socket.h>

namespace bt {

class Player {
public: 
    enum ComStatus {
        SUCCESS,
        FAILURE
    }; 
    /**
     * @brief: The size of the buffer that allocated per player
     */
    const static size_t BUFFER_SIZE = 512; 

    /**
     * @brief: Construct player that is connecting at the specified socket file
     * descriptor
     */
    Player(int socketFd); 

    /**
     * @brief: Get the socket filedescriptor of the player as playerId
     */
    int id() const {
        return d_socketFd; 
    }

    /**
     * @brief: Receive message from the client player and save to buffer
     */
    int receiveMsg(); 

    /**
     * @brief: Send message to the player from the buffer
     */
    int sendMsg(); 

    /**
     * @brief: Add message to the buffer queue to be sent to the client player
     */
    int prepareMsgSend(unsigned char msgId, const char* msg, size_t msgLength); 

    /**
     * @brief: Get message from the buffer
     */
    int readNextMsgReceived(unsigned char* msgId, char* msg); 

private: 
    // storage buffer
    char d_readBuffer[BUFFER_SIZE]; 
    char d_writeBuffer[BUFFER_SIZE]; 
    // buffer length
    size_t d_rbUsed; 
    size_t d_wbUsed; 
    // socket fd
    int d_socketFd; 
    // Socket handler
    ConnectSocket d_socket; 
}; 

    
}


#endif
