import struct

# Define message type
TypeConfig, \
TypeGameReady, \
TypeRequestLevelStart, \
TypeLevelMapData, \
TypeLevelReady, \
TypeLevelStart, \
TypeTankMovement = range(7)


class MsgConfig(object): 

    FORMAT = 'B'

    def __init__(self, data): 
        self.position = data[0]


class MsgLevelMapData(object):
  
    FORMAT = 's'

    def __init__(self, mapData): 
        self.map = mapData


class MsgLevelStart(object): 

    FORMAT = 'L'

    def __init__(self, data): 
        self.startTime = data[0] 


class MsgTankMovement(object): 

    FORMAT = 'BB'

    def __init__(self, tank_id, direction):
        self.id = tank_id 
        self.direction = direction
    
    def serialize(self): 
        return struct.pack(FORMAT, self.id, self.direction)


def deserialize(msgType, msg): 
    """ Convert byte array to Message object """

    if msgType == TypeConfig: 
        print "msg", repr(msg)
        return MsgConfig(struct.unpack(MsgConfig.FORMAT, msg))

    elif msgType == TypeLevelMapData: 
        # As byte are represented as string, don't need to unpack it here
        return MsgLevelMapData(msg)

    elif msgType == TypeLevelStart: 
        return MsgLevelStart(struct.unpack(MsgLevelStart.FORMAT, msg))

    elif msgType == TypeTankMovement:
        return MsgTankMovement(struct.unpack(MsgTankMovement.FORMAT, msg))


