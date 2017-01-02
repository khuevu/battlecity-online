import socket
import select
import errno
import os


class Connection(object): 

    BUFSIZE = 1024

    def __init__(self, host, port): 
        self.host = host
        self.port = port
        # set up a non-blocking socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setblocking(0)
        # Establish connection using connect_ex so that err code returned instead of exception
        # When connecting on non-blocking socket, C connect returns a code to indicate connection in progress, 
        # so the normal connect method will throw unwanted exception
        status_code = self.sock.connect_ex((self.host, self.port))
        if status_code != errno.EINPROGRESS: 
            raise socket.error(status_code, os.strerror(status_code))

    def recv(self): 
        readables, _, _ = select.select([self.sock], [], [], 0) 
        if len(readables) > 0: 
            data = self.sock.recv(self.BUFSIZE)
            print "Receive ", data

    def send(self): 
        pass


