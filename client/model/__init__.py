import pygame


class Drawable(object): 
    """ Represent any object that can be drawn on the screen

    An drawable type can have a Z parameter to change the order 
    in which the object of the type is drawn which influence the
    overlays. 
    
    Its state dictates whether the object is drawed.  
    """

    Z = 0 
    S_ACTIVE, S_DESTROYED = range(2)

    def __init__(self, topleft, size, image):
        """ Initialize the Drawable rectangle area

        Arguments:
        topleft -- (x,y) coordinate of the object
        size -- (width, height) of the object
        image -- the image to be rendered in the rectangle area
        """
        self.__init__(topleft[0], topleft[1],
                size[0], size[1], image)

    def __init__(self, x, y, width, height, image): 
        """ Initialize the Drawable rectangle area

        Arguments:
        x -- the x coordinate of the object
        y -- the y coordinate of the object
        width -- the width of the object
        height -- the height of the object
        image -- the image to be rendered in the rectangle area
        """
        self.__init__(pygame.Rect(x, y, width, height), image)

    def __init__(self, rect, image): 
        """ Initialize the Drawable rectangle area

        Arguments:
        rect -- pygame.Rect which defines the rectangle of the object
        image -- the image to be rendered in the rectangle area
        """
        self.rect = rect
        self.image = image
        self.state = S_ACTIVE
        
    def draw(self, screen): 
        """ Draw object onto the pygame screen. """
        if self.state == S_ACTIVE:
            screen.blit(self.image, self.rect)

    def destroy(self): 
        self.state = S_DESTROYED
        

class GameScreen(object): 
    """ Define the type of game's screens """


