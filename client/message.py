import struct

# Define message type
TypeConfig, \
TypeGameReady, \
TypeRequestLevelStart, \
TypeLevelMapData, \
TypeLevelReady, \
TypeLevelStart, \
TypeTankCreation, \
TypeTankMovement, \
TypeTankAction, \
TypeGameEnd = range(10)


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


class MsgTankCreation(object):

    FORMAT = 'BBBdddii'

    def __init__(self, tank_id,
                 type, direction,
                 x, y,
                 speed, health,
                 power):
        self.id = tank_id
        self.type = type
        self.direction = direction
        self.x = x
        self.y = y
        self.speed = speed
        self.health = health
        self.power = power


class MsgTankMovement(object): 

    FORMAT = 'BBBdd'

    def __init__(self, tank_id, x, y, direction, moving):
        self.id = tank_id
        self.direction = direction
        self.moving = moving
        self.x = x
        self.y = y
    
    def serialize(self): 
        return struct.pack(self.FORMAT, self.id, self.direction, self.moving, self.x, self.y)


class MsgTankAction(object):

    FORMAT = 'BB'

    def __init__(self, tank_id, action):
        self.id = tank_id
        self.action = action

    def serialize(self):
        return struct.pack(self.FORMAT, self.id, self.action)


class MsgGameEnd(object):

    FORMAT = 'B'

    def __init__(self, win):
        self.win = win

    def serialize(self):
        return struct.pack(self.FORMAT, self.win)


def deserialize(msgType, msg): 
    """ Convert byte array to Message object """

    if msgType == TypeConfig: 
        return MsgConfig(struct.unpack(MsgConfig.FORMAT, msg))

    elif msgType == TypeLevelMapData: 
        # As byte are represented as string, don't need to unpack it here
        return MsgLevelMapData(msg)

    elif msgType == TypeLevelStart: 
        return MsgLevelStart(struct.unpack(MsgLevelStart.FORMAT, msg))

    elif msgType == TypeTankCreation:
        tank_id, type, direction, x, y, speed, health, power = struct.unpack(MsgTankCreation.FORMAT, msg)
        return MsgTankCreation(tank_id, type, direction, x, y, speed, health, power)

    elif msgType == TypeTankMovement:
        tank_id, direction, moving, x, y = struct.unpack(MsgTankMovement.FORMAT, msg)
        return MsgTankMovement(tank_id, x, y, direction, moving)

    elif msgType == TypeTankAction:
        print "Msg length", len(msg)
        print "Msg content", msg
        tank_id, action = struct.unpack(MsgTankAction.FORMAT, msg)
        return MsgTankAction(tank_id, action)
