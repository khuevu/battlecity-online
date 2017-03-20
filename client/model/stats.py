from model import Drawable
import image
import pygame


class TankIcon(Drawable):

    SIZE_WIDTH = 13
    SIZE_HEIGHT = 15
    IMAGE = image.tank_icon_enemy

    def __init__(self, x, y):
        Drawable.__init__(self, pygame.Rect((x, y), (self.SIZE_WIDTH, self.SIZE_HEIGHT)), self.IMAGE)


class StatBar(Drawable):

    SIZE_WIDTH = 480 - 416
    SIZE_HEIGHT = 416
    BACKGROUND_COLOR = [100, 100, 100]

    def __init__(self, n_enemy=10):
        loc_x, loc_y = 416, 0
        surface = pygame.Surface((self.SIZE_WIDTH, self.SIZE_HEIGHT))
        surface.fill(self.BACKGROUND_COLOR)
        Drawable.__init__(self, pygame.Rect((416, 0), (self.SIZE_WIDTH, self.SIZE_HEIGHT)), surface)

        self.n_enemy = n_enemy
        self.n_enemy_killed = 0
        start_y = 50
        self.icons = []
        for i in range(n_enemy):
            x = loc_x + 16 if i % 2 == 0 else loc_x + 35
            y = start_y
            self.icons.append(TankIcon(x, y))
            if i % 2 == 1:
                start_y += 18

    def draw(self, screen):
        # Override draw method to draw children of the Drawable
        Drawable.draw(self, screen)
        for icon in self.icons:
            icon.draw(screen)


    def inc_enemy_killed(self):
        self.n_enemy_killed += 1
        self.icons[-self.n_enemy_killed].destroy()

    def all_enemies_killed(self):
        return self.n_enemy == self.n_enemy_killed
