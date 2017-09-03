import image
from model import Drawable, ActiveDrawable
import pygame

class Bullet(ActiveDrawable):

    Z = 300
    SIZE_WIDTH = 6
    SIZE_HEIGHT = 8
    # images in four direction of the bullet
    IMAGES = ActiveDrawable.construct_image_set(image.bullet_img)

    def __init__(self, level, x, y, direction, owner, power):
        ActiveDrawable.__init__(self, pygame.Rect((x, y), (self.SIZE_WIDTH, self.SIZE_HEIGHT))
                                , self.IMAGES, speed=0.2, direction=direction)
        self.level = level
        self.active = True
        self.owner = owner
        self.power = power

    def loop(self, time_passed):
        moved = self.move(time_passed)
        # If bullet can not moved, it should be destroyed
        if not moved:
            self.destroy()

    # def explode(self):
    #     ex_center = self.rect.center
    #     # the dimension doesn't matter here because the explosion is a square.
    #     ex_pos = (ex_center[0] - Explosion.SIZE_WIDTH / 2, ex_center[1] - Explosion.SIZE_HEIGHT / 2)
    #     ex = Explosion(ex_pos)
    #     ex.update(0)

    def move(self, time_passed):
        # Check if the next position in the current direction will lead to collisions
        next_pos, dx, dy = self.calc_next_pos(time_passed)
        # If out of map, can't move
        if not self.level.map.is_within(next_pos):
            return False

        # Check for collision with terrains
        block_terrain = self.level.map.get_block(next_pos, self.direction, on_air=True)
        if block_terrain is not None:
            block_terrain.hit(self)
            return False

        # Check for collision with other tanks
        for tank in self.level.tanks:
            if self.owner != tank and tank.visible() and tank.collide(next_pos):
                tank.hit(self)
                return False

        # Check for collision with other bullets
        for bullet in self.level.bullets:
            if self != bullet and bullet.collide(next_pos):
                # Bullet will automatically destroyed if not moved
                # Other bullet will destry itself.
                return False

        # Check for collision with castle
        if self.level.castle.collide(next_pos):
            self.level.castle.hit(self)
            return False

        self.do_move(dx, dy)
        return True