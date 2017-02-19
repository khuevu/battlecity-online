import pygame
import image
from model import Drawable, Text
from model.screen import GameScreen
import message
from level import Level
from model.map import Map
import time


currenttime_millis = lambda: int(round(time.time() * 1000))


class Stage(object): 

    def __init__(self, game): 
        self.game = game
        self.scrn = GameScreen(game.display) 
        self.server = game.server
        self.clock = pygame.time.Clock()

    def loop(self, time_passed): 
        pass

    def show(self): 
        time_passed = self.clock.tick(40)
        while self.loop(time_passed):
            self.scrn.draw()
            time_passed = self.clock.tick(40)
    
    def next_stage(self): 
        pass

            
class StartStage(Stage): 
    """ Welcome stage """

    def __init__(self, game): 
        Stage.__init__(self, game)
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
                return False
                break
        return True

    def next_stage(self): 
        print "Start connecting to server"
        return JoiningStage(self.game)


class JoiningStage(Stage): 

    STATE_WAIT_PLAYER, STATE_GET_CONF = range(2)

    def __init__(self, game): 
        Stage.__init__(self, game)
        self.notification = Text((80, 200), text="Waiting for other player to join", font_size=16)
        self.scrn.add(self.notification)
        self.state = self.STATE_WAIT_PLAYER

    def loop(self, time_passed): 
        # Wait for other player to join
        msg = self.server.get_message()
        if msg: 
            print "msg_type", msg[0]
            assert msg[0] == message.TypeGameReady
            print "other player joined"
            return False
        else:
            return True

    def next_stage(self):
        print "Starting new level"
        return PrepareLevelStage(self.game)


class PrepareLevelStage(Stage): 

    STATE_NEW, STATE_WAIT_MAP, STATE_WAIT_START = range(3)

    def __init__(self, game): 
        Stage.__init__(self, game)
        self.state = self.STATE_NEW
        self.startTime = None
        self.downloadedMap = None

    def loop(self, time_passed): 
        if self.state == self.STATE_NEW: 
            # Request for game start for map 
            print "Send request to server to start a new level"
            self.server.send_message(message.TypeRequestLevelStart)
            self.state = self.STATE_WAIT_MAP
            
        elif self.state == self.STATE_WAIT_MAP:
            msg = self.server.get_message()
            if msg:
                m_t, m_d = msg
                print "Receive response from server [{}: {}]".format(m_t, m_d)
                assert m_t == message.TypeLevelMapData
                # Construct the level container
                print "Map Data: {}".format(m_d.map)
                self.downloadedMap = m_d.map

                # Send acknowledgement to server
                self.server.send_message(message.TypeLevelReady)
                self.state = self.STATE_WAIT_START

        elif self.state == self.STATE_WAIT_START:
            msg = self.server.get_message()
            if msg: 
                m_t, m_d = msg
                assert m_t == message.TypeLevelStart
                print "Level should starts at", m_d.startTime 
                self.startTime = m_d.startTime
                print "Curren time: ", currenttime_millis()

            elif self.startTime and self.startTime <= currenttime_millis():
                print "Starting new level"
                # stop the current stage
                return False
        return True
                      
    def next_stage(self): 
        return BattleStage(self.game, self.downloadedMap)


class BattleStage(Stage): 

    STATE_RUNNING, STATE_OVER = range(2)

    def __init__(self, game, mapData):
        print "Create Battle Stage"
        Stage.__init__(self, game)
        # construct level
        self.level = Level(self.scrn, self.server, mapData)
       
    def loop(self, time_passed): 
        # call level loop
        return self.level.loop(time_passed) 
        # get the result if loop end
        # decide the next stage
