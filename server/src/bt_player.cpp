#include <iostream>

#include <bt_player.h>

namespace bt {


Player::Player(int socketFd, int playPosition) : 
    d_readBuffer(), d_writeBuffer(), 
    d_rbUsed(0),
    d_socketFd(socketFd), d_socket(d_socketFd), 
    d_position(playPosition) { }


Player::OpStatus Player::receiveMsg() {
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


Player::OpStatus Player::sendMsg(unsigned char msgId, const char *msg, size_t msgLength) {
    // account for header size = msg_length + msg_id
    size_t headerSize = sizeof(unsigned int) + sizeof(unsigned char); 
    if (msgLength + headerSize >= BUFFER_SIZE) {
        std::cerr << "Invalid message. Message is too large. Should not happen " << d_socketFd << std::endl;
        //We can try sending the existing buffer instead of failing here
        return FAILURE; 
    }

    // set message length
    unsigned int sz = msgLength;
    memcpy(&d_writeBuffer[0], &sz, sizeof(sz));
    // set message id
    d_writeBuffer[sizeof(sz)] = msgId;
    // copy msg to buffer
    memcpy(&d_writeBuffer[headerSize], msg, msgLength);

    size_t totalLength = headerSize + msgLength;

    // send the message
    int nbytes = d_socket.sendData(d_writeBuffer, totalLength);
    if (nbytes < 0) {
        std::cerr << "Error sending message to Player at socket " << d_socketFd << std::endl;
        return FAILURE;
    }

    memset(d_writeBuffer, 0, totalLength);
    return SUCCESS;
}

Player::OpStatus Player::readNextMsgReceived(unsigned char* msgId, char* msg) {
    // read next msg from buffer
    if (d_rbUsed == 0) {
        // no msg in buffer
        return NODATA; 
    }

    //size_t length = d_readBuffer[0]; 
    unsigned int length = *((unsigned int*) d_readBuffer); 
    *msgId = d_readBuffer[sizeof(length)]; 
    // copy buffer to msg
    size_t headerSize = sizeof(length) + sizeof(*msgId);
    if (length > 0) {
        memcpy(msg, &d_readBuffer[headerSize], length); 
    }
    // shuffer buffer
    size_t packetLength = headerSize + length; 
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
