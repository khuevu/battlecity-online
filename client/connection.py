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
            data = readables[0].recv(self.BUFSIZE)
            if len(data) == 0:
                # if select return socket as ready but the data length is 0, probably the other end has hung up
                raise socket.error("Disconnected")

            return data
        else:
            return None

    def send(self): 
        pass


# Define message type
MsgTypeGameReady, \
MsgTypeMapData = range(2)


class Server(object): 

    def __init__(self, conn): 
        self.connection = conn

    def get_message(self): 
        data = self.connection.recv()
        if not data:
            return None
        print "Recv", data

        # parse the data into individual messages
        msg_type = int(data[0])
        msg_length = int(data[1])

        return (msg_type, None)

