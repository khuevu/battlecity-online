from model.screen import GameScreen
from model.map import Map
from model.tank import PlayerTank, PartnerTank
import message


class Level(object): 

    def __init__(self, gameScreen, server, mapData, playerPosition): 
        self.server = server
        self.playerPosition = playerPosition
        # screen and engine should be private and updated through the add method
        self.scrn = gameScreen 
        # Level objects
        # Player tanks
        self.player = None
        self.partner = None
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
        if self.player:
            self.player.loop(time_passed)

        if self.partner: 
            self.partner.loop(time_passed)

        # process server update
        self._process_server_msg()
        return True

    def _process_server_msg(self): 
        while True: 
            msg = self.server.get_message()
            if msg is not None: 
                msg_type, msg_data = msg
                # process 
                if msg_type == message.TypeTankCreation: 
                    self._create_tank(msg_data)

                elif msg_type == message.TypeTankMovement:
                    self._update_tank(msg_data)

                else:
                    print msg
            else: 
                break
        
    def _create_tank(self, data): 
        tank_id = data.id

        if tank_id <= 2: # create players' tanks
            print "Create player and partner tank"
            if tank_id == self.playerPosition:
                # create current player tank
                self.player = PlayerTank(self, tank_id, data.x, data.y)
            else:
                self.partner = PartnerTank(self, tank_id, data.x, data.y)
        else: 
            print "Create enemy tanks"
            pass
    

    def _update_tank(self, data): 
        tank_id = data.id
        if tank_id <= 2: # Update partner tank
            self.partner.update(data.x, data.y, data.direction, data.action)
        else: 
            # Update enemy tank
            pass
        
