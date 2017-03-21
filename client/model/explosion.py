import image
from model import Drawable
import pygame


class Explosion(Drawable):

    Z = 300
    SIZE = 64
    FRAME_DUR = 90

    def __init__(self, x, y, large=False):
        self.images = image.explosion_imgs
        self.n_stages = len(self.images) if large else len(self.images) - 1

        # self.level = level
        # self.sound = sound
        Drawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), self.images[0])
        # Explostion duration
        self.duration = 0
        self.active = True

    def loop(self, time_passed):
        self.duration += time_passed
        stage = self.duration / self.FRAME_DUR
        if stage >= self.n_stages:
            self.active = False
            self.destroy()

        else:
            self.image = self.images[stage]

    def destroyed(self):
        return not self.active


def explode(drawable):
    ex_center = drawable.center
    ex_x, ex_y = ex_center[0] - Explosion.SIZE / 2, ex_center[1] - Explosion.SIZE / 2
    return Explosion(ex_x, ex_y)