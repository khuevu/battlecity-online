import struct

# Define message type
TypeGameReady, \
TypeRequestLevelStart, \
TypeLevelMapData, \
TypeLevelReady, \
TypeLevelStart, \
TypePlayerPosition, \
TypeOtherPlayerPosition, \
TypeEnemyPosition = range(8)


#class MsgRequestLevelStart(object): 

    #MSG_TYPE = TypeRequestLevelStart

    #def __init__(self): 
        #pass

    #def serialize(self): 
        #return b''


class MsgLevelMapData(object):
  
    FORMAT = 's'

    def __init__(self, mapData): 
        self.map = mapData


#class MsgLevelReady(object):

    #MSG_TYPE = TypeLevelReady

    #def __init__(self): 
        #pass

    #def serialize(self): 
        #return b''


class MsgLevelStart(object): 

    FORMAT = 'L'

    def __init__(self, data): 
        self.startTime = data[0] 


class MsgTankPosition(object): 

    FORMAT = 'Bdd'

    def __init__(self, tank_id, x, y):
        self.id = tank_id 
        self.x = x
        self.y = y
    
    def serialize(self): 
        return struct.pack(FORMAT, self.id, self.x, self.y)


#class MsgOtherPlayerPosition(MsgTankPosition): 

    #MSG_TYPE = TypeOtherPlayerPosition


#class MsgPlayerPosition(MsgTankPosition): 

    #MSG_TYPE = TypePlayerPosition


#class MsgEnemyPosition(MsgTankPosition): 

    #MSG_TYPE = TypeEnemyPosition


def deserialize(msgType, msg): 
    """ Convert byte array to Message object """

    print "Deserialize ", msgType
    if msgType == TypeLevelMapData: 
        # As byte are represented as string, don't need to unpack it here
        return MsgLevelMapData(msg)

    elif msgType == TypeLevelStart: 
        return MsgLevelStart(struct.unpack(MsgLevelStart.FORMAT, msg))

    elif msgType == TypePlayerPosition or msgType == TypeEnemyPosition or msgType == TypeOtherPlayerPosition:
        return MsgTankPosition(struct.unpack(MsgTankPosition.FORMAT, msg))


