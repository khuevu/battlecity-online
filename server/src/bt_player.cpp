#include <iostream>

#include <bt_player.h>

namespace bt {
Player::Player(int socketFd) : 
    d_readBuffer(), d_writeBuffer(), 
    d_rbUsed(0), d_wbUsed(0),
    d_socketFd(socketFd), d_socket(d_socketFd) { }

int Player::receiveMsg() {
    // we might want to add checksum
    int nbytes = d_socket.receiveData(d_readBuffer + d_rbUsed, BUFFER_SIZE - d_rbUsed); 
    if (nbytes < 0) {
        std::cerr << "Error reading the next message from Player at socket " << d_socketFd << std::endl;
        return FAILURE; 
    }
    // set the length of the used buffer
    d_rbUsed += nbytes / sizeof(d_readBuffer[0]); 
    return SUCCESS; 
}

int Player::sendMsg() {
    // send message from buffer
    int nbytes = d_socket.sendData(d_writeBuffer, d_wbUsed); 
    if (nbytes < 0) {
        std::cerr << "Error sending message to Player at socket " << d_socketFd << std::endl; 
        return FAILURE; 
    }
    // reset buffer
    d_wbUsed = 0; 
    memset(d_writeBuffer, 0, BUFFER_SIZE); 
    return SUCCESS; 
}

int Player::prepareMsgSend(unsigned char msgId, const char* msg, size_t msgLength) {
    // account for header
    if (msgLength + 2 >= BUFFER_SIZE - d_wbUsed) {
        std::cerr << "No buffer available for Player at socket " << d_socketFd << std::endl;
        std::cerr << "Required: " << msgLength << ". But only available: " << (BUFFER_SIZE - d_wbUsed) << std::endl;
        //We can try sending the existing buffer instead of failing here
        return FAILURE; 
    }
    
    char* availableWriteBuffer = d_writeBuffer + d_wbUsed; 
    availableWriteBuffer[0] = (unsigned char) msgLength; 
    availableWriteBuffer[1] = msgId; 
    // copy msg to buffer
    memcpy(&availableWriteBuffer[2], msg, msgLength); 
    d_wbUsed += msgLength + 2; 
    return SUCCESS; 
}

int Player::readNextMsgReceived(unsigned char* msgId, char* msg) {
    // read next msg from buffer
    if (d_rbUsed == 0) {
        // no msg in buffer
        return FAILURE; 
    }

    size_t length = d_readBuffer[0]; 
    *msgId = d_readBuffer[1]; 
    // copy buffer to msg
    memcpy(msg, &d_readBuffer[2], length); 
    // shuffer buffer
    size_t packetLength = length + 2; 
    if (d_rbUsed == packetLength) {
        // clear the buffer if all the packets have been read
        memset(&d_readBuffer, 0, BUFFER_SIZE); 
    } else {
        // there is still msg to read. Shift the next msg to the begining of
        // the buffer
        memcpy(&d_readBuffer, &d_readBuffer[packetLength], d_rbUsed - packetLength);  
    }
    // reduced the buffered used size
    d_rbUsed -= packetLength;  
    return SUCCESS; 
}

}
