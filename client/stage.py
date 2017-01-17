import pygame
import image
from model import Drawable, Text
from model.screen import GameScreen
from connection import Server, Connection


class Stage(object): 

    def __init__(self, display): 
        self.scrn = GameScreen(display) 
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

    def __init__(self, game): 
        Stage.__init__(self, game.display)
        self.game = game
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
                print "Continue the game"
                self.running = False  
                break

    def next_stage(self): 
        return JoiningStage(self.game)


class JoiningStage(Stage): 

    STATE_WAIT_PLAYER, STATE_GET_CONF = range(2)

    def __init__(self, game): 
        Stage.__init__(self, game.display)
        self.game = game
        self.notification = Text((80, 200), text="Waiting for other player to join", font_size=16)
        self.scrn.add(self.notification)
        # connect to server
        self.server = Server(Connection(game.serverAddr[0], game.serverAddr[1]))
        self.state = self.STATE_WAIT_PLAYER

    def loop(self, time_passed): 
        while True: 
            # Wait for other player to join
            print "Wait for other player to join"
            data = self.server.get_message()
            if data: 
                msg_type, msg = data[0], data[1]
                
                print "Server ready"
                break
        # Download configuration
        # Move on to level when ready
