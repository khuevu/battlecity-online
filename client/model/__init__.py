import pygame
import font


class Drawable(object): 
    """ Represent any object that can be drawn on the screen

    An drawable type can have a Z parameter to change the order 
    in which the object of the type is drawn which influence the
    overlays. 
    
    Its state dictates whether the object is drawed.  
    """

    Z = 0 
    S_ACTIVE, S_DESTROYED = range(2)

    def __init__(self, rect, image): 
        """ Initialize the Drawable rectangle area

        Arguments:
        rect -- pygame.Rect which defines the rectangle of the object
        image -- the image to be rendered in the rectangle area
        """
        self.rect = rect
        self.image = image
        self.state = self.S_ACTIVE
        
    def draw(self, screen): 
        """ Draw object onto the pygame screen. """
        if self.state == self.S_ACTIVE:
            screen.blit(self.image, self.rect)

    def destroy(self): 
        self.state = self.S_DESTROYED
        

class Text(Drawable):
    """ Text Object that can be rendered on the Screen. """

    Z = 0

    def __init__(self, topleft, text="", 
            font_size=16, color=pygame.Color('white')):
        """ Construct a Text Image at the specified topleft position. """
        self.fontSize = font_size
        self.text = text
        self.color = color
        img, size = self._render_as_image(text)
        Drawable.__init__(self, pygame.Rect(topleft, size), img)

    def _render_as_image(self, text): 
        f = font.with_size(self.fontSize)
        # Determine size
        size = f.size(text)

        image = f.render(text, False, self.color)
        return image, size

    def set_content(self, text): 
        """ Update the content of the Text Image. """
        img, size = self._render_as_image(text)
        # Modify the Drawable attributes based on text attributes
        self.image = img
        self.rect.size = size


#class AnimatedDrawable(Drawable): 
    
    #def __init__(self, rect, image): 
        #Drawable.__init__(self, rect, image)

    #def loop(time_passed=None):  
        #pass

