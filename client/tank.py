import pygame
import os
from connection import Connection


class Game(object): 

    def __init__(self, server_addr): 
        self.connection = Connection(server_addr[0], server_addr[1])

    def start(self): 
        self.connection.recv()

    def over(self):
        pass

if __name__ == "__main__": 
    # Initialize pygame engine
    os.environ['SDL_VIDEO_WINDOW_POS'] = 'center'
    pygame.init()
    pygame.display.set_caption("Multiplayer Battle City Game")

    # initialize image resources
    import image
    pygame.display.set_icon(image.game_icon)

    # initialize font resource
    import font
    
    # initialize sound resources
    pygame.mixer.init(frequency=44100, size=-16, channels=1, buffer=1024)
    import sound

    # start game
    game = Game(server_addr=("localhost", 9034))
    print "Start polling data from server"
    while True:
        game.start()

