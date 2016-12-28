#ifndef INCLUDED_BT_PLAYER
#define INCLUDED_BT_PLAYER

#include <cstdlib>

#include <bt_socket.h>

namespace bt {

class Player {
public: 

    /**
     * @brief: The size of the buffer that allocated per player
     */
    const static size_t INIT_BUFFER_SIZE = 1024; 

    /**
     * @brief: Construct player that is connecting at the specified socket file
     * descriptor
     */
    Player(int socketFd); 

    /**
     * @brief: Return the begining of the available buffer
     */
    char* buffer(); 

    /**
     * @brief: Return the size of the available buffer
     */
    size_t bufferSize() const; 

    /**
     * @brief: Get the socket filedescriptor of the player
     */
    int socketFd() const {
        return d_socketFd; 
    }

    /**
     * @brief: Get the next message from the player buffer
     */
    int readNextMsg(char* msg); 

    /**
     * @brief: Send message to the player
     */
    int sendMsg(const char* msg); 

private: 
    // storage buffer
    char d_buffer[2014]; 
    // buffer length
    size_t d_bufferLength; 
    // socket fd
    int d_socketFd; 
    // Socket handler
    ConnectSocket d_socket; 
}; 

inline char* Player::buffer() {
    return d_buffer + d_bufferLength; 
}

inline size_t Player::bufferSize() const {
    return 2014 - d_bufferLength; 
}
    
}


#endif
