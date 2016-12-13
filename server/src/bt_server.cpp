#include <bt_server.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

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

Server::Server(int port) : d_port(port), d_numPlayer(0) {
}

void Server::init() {
    //struct sockaddr_storage remoteaddr; // client address
    //socklen_t addrlen;

    //char buf[256];    // buffer for client data
    //int nbytes;

    //char remoteIP[INET6_ADDRSTRLEN];

    //FD_ZERO(&master);    // clear the master and temp sets
    //FD_ZERO(&read_fds);

    struct addrinfo hints, *ai, *p;
    memset(&hints, 0, sizeof hints);
    // get us a socket and bind it
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rv;
    if ((rv = getaddrinfo(NULL, std::to_string(d_port).c_str(), &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    //loop through available addresses and try to create socket
    for(p = ai; p != NULL; p = p->ai_next) {
        d_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (d_socket < 0) { 
            continue;
        }
        // enable reuse of socket  
        int yes=1;        // for setsockopt() SO_REUSEADDR, below
        setsockopt(d_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(d_socket, p->ai_addr, p->ai_addrlen) < 0) {
            close(d_socket);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(d_socket, 10) == -1) {
        perror("listen");
        exit(3);
    } 
}

void Server::waitForPlayersToJoin() {
    fd_set read_fds; 
    FD_ZERO(&read_fds); // clear readset

    FD_SET(d_socket, &read_fds); 
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr; 

    while (d_numPlayer < 2) {
        if (select(d_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        // check if there is new connection
        if (FD_ISSET(d_socket, &read_fds)) {
            // addrlen initially is the space of remoteaddr
            // upon return it should contains the number of bytes of the actual
            // remote addr
            int newfd = accept(d_socket,
                (struct sockaddr *)&remoteaddr,
                &addrlen);

            if (newfd == -1) {
                perror("accept");
            } else {
                char remoteIP[INET6_ADDRSTRLEN]; 
                printf("selectserver: new connection from %s on "
                    "socket %d\n",
                    inet_ntop(remoteaddr.ss_family,
                        get_in_addr((struct sockaddr*)&remoteaddr),
                        remoteIP, INET6_ADDRSTRLEN),
                    newfd);
                
                std::cout << "New player joined " << std::endl; 
                d_numPlayer += 1; 
            }
        }
    }
}


}
