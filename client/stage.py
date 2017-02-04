import pygame
import image
from model import Drawable, Text
from model.screen import GameScreen
from connection import Server


class Stage(object): 

    def __init__(self, display, connection): 
        self.scrn = GameScreen(display) 
        self.server = Server(connection)
        self.running = True
        self.clock = pygame.time.Clock()
        self.nextStage = None

    def loop(self, time_passed): 
        pass

    def show(self): 
        while self.running:
            self.scrn.draw()
            time_passed = self.clock.tick(40)
            self.loop(time_passed)
    
    def next_stage(self): 
        return self.nextStage 

            
class StartStage(Stage): 
    """ Welcome stage """

    def __init__(self, display, connection): 
        Stage.__init__(self, display, connection)
        # Display welcome text
        battleText = Text((80, 100), text="Battle City", font_size=30)
        self.scrn.add(battleText)

        # Display start option
        startText = Text((100, 200), text="Press Enter to start", font_size=16)
        self.scrn.add(startText)
        tankIcon = Drawable(pygame.Rect((100, 250), (32, 32)), image.tank_player)
        self.scrn.add(tankIcon)

    def loop(self, time_passed): 
       	for event in pygame.event.get():
            if event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
                print "Player chose to continue the game"
                self.running = False  
                break

    def next_stage(self): 
        print "Start connecting to server"
        return JoiningStage(self.scrn.display, self.server.connection)


class JoiningStage(Stage): 

    STATE_WAIT_PLAYER, STATE_GET_CONF = range(2)

    def __init__(self, display, connection): 
        Stage.__init__(self, display, connection)
        self.notification = Text((80, 200), text="Waiting for other player to join", font_size=16)
        self.scrn.add(self.notification)
        self.state = self.STATE_WAIT_PLAYER

    def loop(self, time_passed): 
        # Wait for other player to join
        msg = self.server.get_message()
        if msg: 
            print "msg_type", msg[0]
            assert msg[0] == MsgTypeGameReady
            print "other player joined"
            self.running = False

    def next_stage(self):
        print "Starting new level"
        return BattleStage(self.scrn.display, self.server.connection)


class BattleStage(Stage): 

    STATE_NEW, STATE_WAIT_MAP, STATE_WAIT_START, \
    STATE_READY, STATE_RUNNING, STATE_WIN, STATE_OVER = range(7)

    def __init__(self, display, connection, level=1): 
        Stage.__init__(self, display, connection)
        self.state = STATE_NEW
        self.level = level

    def loop(self, time_passed): 
        if self.state == STATE_NEW: 
            # Request for game start for map 
            print "Send request to server to start a new level"
            self.server.send_message(MsgRequestLevelStart(level))
            self.state = STATE_WAIT_MAP
            
        elif self.state == STATE_WAIT_MAP:
            msg = self.server.get_message()
            if msg:
                m_t, m_d = msg
                print "Receive response from server [{}: {}]".format(m_t, m_d)
                assert m_t == MsgTypeLevelMapData
                # Construct the level container
                print "Map Data: {}".format(m_d.map)

                # Send acknowledgement to server
                self.server.send_message(MsgLevelReady())
                self.state = STATE_WAIT_START

        elif self.state == STATE_WAIT_START:
            msg = self.server.get_message()
            if msg: 
                m_t, m_d = msg
                assert m_t == MsgTypeLevelStart
                print "Level should starts at %d" % m_d.startTime
