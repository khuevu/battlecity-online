import pygame
import image
from model import Drawable, Text


class Stage(object): 

    def __init__(self, game_screen): 
        self.scrn = game_screen
        self.running = True
        self.clock = pygame.time.Clock()
        self.nextStage = None

    def loop(self, time_passed): 
        pass

    def show(self): 
        while self.running:
            time_passed = self.clock.tick()
            self.loop(time_passed)
            self.scrn.draw()
    
    def next_stage(self): 
        return self.nextStage 

            
class StartStage(Stage): 

    def __init__(self, game_screen): 
        Stage.__init__(self, game_screen)
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
                #self.nextStage = 
                break

