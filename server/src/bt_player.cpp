#include <bt_player.h>

namespace bt {
static int test = 1; 
Player::Player(int socketFd) : d_buffer(), d_bufferLength(0), d_socketFd(socketFd), d_socket(d_socketFd) { }

int Player::readNextMsg(char* msg) {
    // skip checksum because the msg will be small
    // TODO: might check chessgame hash and base64 method. 
    
    // verify message in the buffer
     
    // extract the next message, shift the buffer
    return 0; 
}

int Player::sendMsg(const char* msg) {
    return 0; 
}

}
