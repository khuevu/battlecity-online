#ifndef INCLUDED_BT_SOCKET
#define INCLUDED_BT_SOCKET
#include <cstdio>

namespace bt {

class Server; 

class Socket {
public:
    /**
     * @brief: Start the new listening socket
     * @param[in] port: Port number the socket binds to
     */
    Socket(int port); 

    /**
     * @brief: Check for new connection on the socket
     * @return: new accepted socket file descriptor id; -1 if there is no new
     * connection
     */
    int receiveNewConnection(); 

    /**
     * @brief: Receive data from the given socket if any
     * @param[in] fd: FileDescriptor of the socket to receive data from
     * @param[in] buffer: buffer to store the received data
     * @param[in] buffersize: buffer size
     * @return: number of bytes received, or negative value if error occurs
     */
    int receiveData(int fd, char* buffer, size_t bufferSize); 

    /**
     * @brief: Send data to a socket
     * @param[in] fd: FileDescriptor of the socket to send data to
     * @param[in] buffer: Data buffer to send
     * @param[in] sendLength: Length of the data to be sent
     * @return: Status code
     */
    int sendData(int fd, const char* buffer, size_t sendLength); 
        
    int d_port; 
    int d_listenerfd; 
};
}
#endif
