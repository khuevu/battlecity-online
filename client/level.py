from model.screen import GameScreen
from model.map import Map
from model.tank import PlayerTank
import message


class Level(object): 

    def __init__(self, gameScreen, server, mapData, playerPosition): 
        self.server = server
        # screen and engine should be private and updated through the add method
        self.scrn = gameScreen 
        # Level objects
        # Player tanks
        self.player = PlayerTank(level=self, position=playerPosition)
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
        # process actors
        self.player.loop(time_passed)

        # process server update
        self._process_server_msg()
        return True

    def _process_server_msg(self): 
        while True: 
            msg = self.server.get_message()
            if msg is not None: 
                msg_type, msg_data = msg
                # process 
                if msg_type == message.TypeTankMovement:
                    print msg_data
                else:
                    print msg
            else: 
                break
        

    def _update_tank_position(self, tank): 
        pass
        
