import socket
import select
import errno
import os
import struct
from collections import deque
import message


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

    def send(self, data): 
        self.sock.sendall(data)


class Server(object): 

    def __init__(self, conn): 
        self.connection = conn
        # keep incomplete package
        self.buffer = '' 
        self.msgs = deque()

    def _extract_buffered_messages(self): 
        i = 0
        while i < len(self.buffer): 
            # read message length and type
            headerSize = 5
            length, msgType = struct.unpack('IB', self.buffer[i : i + headerSize])
            print "Conn: received msg type {} of length {}".format(msgType, length) 
            # check if the complete msg has been received
            if i + headerSize + length <= len(self.buffer): 
                # read the msg
                msg = message.deserialize(msgType, self.buffer[i + headerSize : i + headerSize + length])
                i += headerSize + length 
                self.msgs.append([msgType, msg])
            else: 
                # shift up the buffer and break
                if i > 0: 
                    self.buffer = self.buffer[i:]
                break

        return self.msgs

    def get_message(self): 
        if len(self.msgs) == 0: 
            # receive new data from connection 
            new_data = self.connection.recv()
            if new_data:
                # add new data to buffer
                #self.buffer.extend(new_data)
                self.buffer += new_data
                # extract the message to the pending msg queue
                self._extract_buffered_messages()
    
        if len(self.msgs) > 0: 
            return self.msgs.popleft()
        else:
            return None

    def send_message(self, msg): 
        data = struct.pack('BBs', len(msg) + 1, msg.MSG_TYPE, msg.serialize()) 
        self.connection.send(data)
