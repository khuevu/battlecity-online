import pygame
import os
from connection import Connection
import stage
from model.screen import GameScreen


class Game(object): 

    def __init__(self, scrn, server_addr): 
        self.gameScreen = GameScreen(scrn)
        #self.connection = Connection(server_addr[0], server_addr[1])

    def start(self): 
        #self.connection.recv()
        cur_stage = stage.StartStage(self.gameScreen)
        cur_stage.show()


if __name__ == "__main__": 
    # Initialize pygame engine
    os.environ['SDL_VIDEO_WINDOW_POS'] = 'center'
    pygame.init()
    pygame.display.set_caption("Multiplayer Battle City Game")
    screen = pygame.display.set_mode((480, 416))

    # import image resources
    import image
    pygame.display.set_icon(image.game_icon)

    # import font resource
    import font
    
    # import sound resources
    import sound

    # start game
    game = Game(scrn=screen, server_addr=("localhost", 9034))
    print "Start polling data from server"
    while True:
        game.start()

