from model.screen import GameScreen
from model.map import Map


class Level(object): 

    def __init__(self, gameScreen, server, mapData): 
        self.server = server
        # engine which will invoke loop on list of object
        self.engine = None
        # screen and engine should be private and updated through the add method
        self.scrn = gameScreen 
        # Level objects
        # Player tanks
        # Enemy tanks
        # Map
        self.map = Map(mapData)

        # Add the object to be drawn
        self.scrn.add(self.map)
        
    def loop(self, time_passed): 
        # run engine
        return True

    def processServerMsg(self): 
        pass
        
