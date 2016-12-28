#ifndef INCLUDED_BT_SOCKET
#define INCLUDED_BT_SOCKET
#include <cstdio>

namespace bt {

class ListenSocket {
public:
    /**
     * @brief: Start the new listening socket
     * @param[in] port: Port number the socket binds to
     */
    ListenSocket(int port); 

    /**
     * @brief: Check for new connection on the socket
     * @return: new accepted socket file descriptor id; -1 if there is no new
     * connection
     */
    int receiveNewConnection(); 

private:
    // listening port
    int d_port; 
    // file descriptor
    int d_listenerfd; 
};

class ConnectSocket {
public: 
    ConnectSocket(int fd); 

    /**
     * @brief: Receive data from the given socket if any
     * @param[in] buffer: buffer to store the received data
     * @param[in] buffersize: buffer size
     * @return: number of bytes received, or negative value if error occurs
     */
    int receiveData(char* buffer, size_t bufferSize); 

    /**
     * @brief: Send data to a socket
     * @param[in] buffer: Data buffer to send
     * @param[in] sendLength: Length of the data to be sent
     * @return: Status code
     */
    int sendData(const char* buffer, size_t sendLength); 

private:
    int d_fd; 

};
}
#endif
