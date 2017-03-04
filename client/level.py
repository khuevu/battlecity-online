from model.screen import GameScreen
from model.map import Map
from model.tank import PlayerTank


class Level(object): 

    def __init__(self, gameScreen, server, mapData, playerPosition): 
        self.server = server
        # screen and engine should be private and updated through the add method
        self.scrn = gameScreen 
        # Level objects
        # Player tanks
        self.player = PlayerTank(level=self, tankId=1, position=playerPosition)
        self.otherPlayer = None
        # Enemy tanks
        self.enemies = []
        # Bullets
        self.bullets = []
        # Map
        self.map = Map(self, mapData)
        #self.scrn.add(self.map)
        # Add the object to be drawn

    def register(self, obj): 
        self.scrn.add(obj)
        
    def loop(self, time_passed): 
        self.player.loop(time_passed)
        return True

    def processServerMsg(self): 
        pass
        
