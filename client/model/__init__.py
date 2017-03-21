import pygame
import font


class Drawable(object): 
    """ Represent any object that can be drawn on the screen

    An drawable type can have a Z parameter to change the order 
    in which the object of the type is drawn which influence the
    overlays. 
    
    Its state dictates whether the object will be drawn.  
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

    def destroyed(self):
        return self.state == self.S_DESTROYED

    @property
    def x(self): 
        return self.rect.x

    @x.setter
    def x(self, value): 
        self.rect.x = value

    @property
    def y(self): 
        return self.rect.y
        
    @y.setter
    def y(self, value): 
        self.rect.y = value

    @property
    def center(self):
        return self.rect.center

    def collide(self, rect):
        return self.rect.colliderect(rect)


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


class ActiveDrawable(Drawable): 
    """ Object that can be rendered on the screen and its position can be changed over time."""

    # Four basic directions
    DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT = range(4)
    DIR_STOP = -1
    # Rotate from DIR_UP 90 degree everytime to get the other direction in order
    DIR_SEP = 90
    # Vectors indicate the direction 
    DIR_VECTORS = [[0, -1], [-1, 0], [0, 1], [1, 0]]

    @staticmethod
    def is_opposite(d1, d2):
        return abs(d1 - d2) == 2

    @staticmethod
    def construct_image_set(up_image): 
        return [up_image,
                pygame.transform.rotate(up_image, 90),
                pygame.transform.rotate(up_image, 180),
                pygame.transform.rotate(up_image, 270)]


    def __init__(self, rect, images, speed, direction=DIR_UP): 
        """ Construct the object 
        Parameters: 
        - images: list of object image in different directions in up, left, down, right order
        - speed: the speed per frame of the object
        - direction: initial direction of the object
        """
        assert len(images) == len(self.DIR_VECTORS)
        self.cached_images = images
        self.direction = direction
        self.speed = speed
        Drawable.__init__(self, rect, self.cached_images[direction])

    def calc_next_pos(self, time_passed): 
        """ Calculate the next rectangular position of the object by moving along 
        the current direction. The rectangle is returned together with the delta x
        and delta y from the topleft of the old position.
        """
        vx, vy = self.DIR_VECTORS[self.direction]
        dx, dy = vx * self.speed * time_passed, vy * self.speed * time_passed
        next_pos = self.rect.move(dx, dy)
        return next_pos, dx, dy

    def do_move(self, dx, dy):
        """ Perform the movement in current direction by dx, dy. """
        self.rect.move_ip(dx, dy)

    def move(self, direction): 
        """ Move the object in the given direction. The logic should be implemented 
        by the subclass. 
        """
        pass

    def rotate(self, direction): 
        """ Rotate the object. """
        if self.direction == direction: 
            return

        self.image = self.cached_images[direction]
        # If rotation is not in the opposite direction, reposition the rect topleft corner
        if not self.is_opposite(direction, self.direction):
            old_center = self.rect.center
            self.rect.width, self.rect.height = self.rect.width, self.rect.height
            self.rect.center = old_center

        self.direction = direction
