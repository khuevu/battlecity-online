import struct

# Define message type
MsgTypeGameReady, \
MsgTypeRequestLevelStart, \
MsgTypeLevelMapData, \
MsgTypeLevelReady, \
MsgTypeLevelStart = range(5)


class MsgRequestLevelStart(object): 

    MSG_TYPE = MsgTypeRequestLevelStart

    def __init__(self): 
        pass

    def serialize(self): 
        return b''


class MsgLevelMapData(object):
  
    FORMAT = 's'
    MSG_TYPE = MsgTypeLevelMapData

    def __init__(self, mapData): 
        self.map = mapData


class MsgLevelReady(object):

    MSG_TYPE = MsgTypeLevelReady

    def __init__(self): 
        pass

    def serialize(self): 
        return b''


class MsgLevelStart(object): 

    FORMAT = 'L'
    MSG_TYPE = MsgTypeLevelStart

    def __init__(self, data): 
        self.startTime = data[0] 


def deserialize(msgType, msg): 
    """ Convert byte array to Message object """

    print "Deserialize ", msgType
    if msgType == MsgTypeLevelMapData: 
        # As byte are represented as string, don't need to unpack it here
        return MsgLevelMapData(msg)

    elif msgType == MsgTypeLevelStart: 
        return MsgLevelStart(struct.unpack(MsgLevelStart.FORMAT, msg))


