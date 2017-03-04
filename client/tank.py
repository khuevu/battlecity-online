import pygame
import os
import stage
from connection import Connection, Server


class Game(object): 

    def __init__(self, display, server_addr): 
        self.display = display
        self.serverAddr = server_addr
        self.server = Server(Connection(server_addr[0], server_addr[1]))
        self.playerPosition = None


if __name__ == "__main__": 
    # Initialize pygame engine
    os.environ['SDL_VIDEO_WINDOW_POS'] = 'center'
    pygame.init()
    pygame.display.set_caption("Multiplayer Battle City Game")
    display = pygame.display.set_mode((480, 416))

    # import image resources
    import image
    pygame.display.set_icon(image.game_icon)

    # import font resource
    import font
    
    # import sound resources
    import sound

    # start game
    game = Game(display=display, server_addr=("localhost", 9034))
    cur_stage = stage.StartStage(game)
    while cur_stage: 
        cur_stage.show() # execute the stage
        cur_stage = cur_stage.next_stage()
