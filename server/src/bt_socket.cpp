#include <iostream>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cassert>

#include <bt_socket.h>

namespace {
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
}

namespace bt {

// --- ListenSocket ---
//
ListenSocket::ListenSocket(int port) : d_port(port) 
{
    struct addrinfo hints, *ai, *p;
    memset(&hints, 0, sizeof hints);
    // get us a socket and bind it
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rv;
    if ((rv = getaddrinfo(NULL, std::to_string(d_port).c_str(), &hints, &ai)) != 0) {
        std::cerr << "Select server: " << gai_strerror(rv) << std::endl; 
        exit(1);
    }
    
    //loop through available addresses and try to create socket
    for(p = ai; p != NULL; p = p->ai_next) {
        d_listenerfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (d_listenerfd < 0) { 
            continue;
        }
        // enable reuse of socket  
        int yes=1;        // for setsockopt() SO_REUSEADDR, below
        setsockopt(d_listenerfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(d_listenerfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(d_listenerfd);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        //fprintf(stderr, "selectserver: failed to bind\n");
        std::cerr << "Select server: failed to bind" << std::endl; 
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(d_listenerfd, 10) == -1) {
        perror("listen");
        exit(3);
    }
} 

int ListenSocket::receiveNewConnection() 
{
    fd_set read_fds; 
    FD_ZERO(&read_fds); // clear readset
    FD_SET(d_listenerfd, &read_fds); 
    
    int newfd = -1; 
    if (select(d_listenerfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
        perror("select");
        exit(4);
    }

    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr; 
    // check if there is new connection
    if (FD_ISSET(d_listenerfd, &read_fds)) {
        // addrlen initially is the space of remoteaddr
        // upon return it should contains the number of bytes of the actual
        // remote addr
        newfd = accept(d_listenerfd,
            (struct sockaddr *)&remoteaddr,
            &addrlen);

        if (newfd == -1) {
            perror("accept");
        } else {
            char remoteIP[INET6_ADDRSTRLEN]; 
            std::cout << "selectserver: new connection from " <<
                    inet_ntop(remoteaddr.ss_family,
                    get_in_addr((struct sockaddr*)&remoteaddr),
                    remoteIP, INET6_ADDRSTRLEN) << 
                    " on socket " << newfd << std::endl; 
        }
    }
    return newfd; 
}


// --- ConnectSocket ---

ConnectSocket::ConnectSocket(int fd) : d_fd(fd) {}

int ConnectSocket::receiveData(char* buffer, size_t bufferSize) 
{
    fd_set read_fds; 
    FD_ZERO(&read_fds); // clear readset
    FD_SET(d_fd, &read_fds); 

    // set the timeval to 0 to effect a poll
    struct timeval tv; 
    tv.tv_sec = 0; 
    tv.tv_usec = 0; 
        
    if (select(d_fd + 1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("select"); 
        exit(4); 
    }

    int nbytes = 0; 
    if (FD_ISSET(d_fd, &read_fds)) {
        if ((nbytes = recv(d_fd, buffer, bufferSize, 0)) <= 0) {
            // if no bytes received
            if (nbytes == 0) {
                std::cout << "Socket " << d_fd << " hung up" << std::endl; 
            } else {
                perror("recv"); 
            }
            // close socket
            close(d_fd); 
        } 
    }

    return nbytes; 
}


int ConnectSocket::sendData(const char* buffer, size_t sendLength)
{
    // do nothing if send length is invalid
    if (sendLength <= 0) return sendLength; 

    int nbytesSent = 0; 
    if ((nbytesSent = send(d_fd, buffer, sendLength, 0)) == -1) {
        perror("send");  
    }
    
    return nbytesSent; 
}

}
