import image
from model import Drawable, ActiveDrawable
import pygame


class Tank(ActiveDrawable):

    Z = 100
    SIZE = 26

    def __init__(self, level, tankId, x, y, images, speed, 
            health, power, direction=ActiveDrawable.DIR_UP):
        ActiveDrawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), images, speed, direction)
        self.id = tankId
        self.level = level
        self.health = health
        self.power = power
        self.level.register(self) 

    def move(self, direction, time_passed):
        self.rotate(direction) 
        # Check if the next position in the current direction will lead to collisions
        next_pos, dx, dy = self.calc_next_pos(time_passed)

        # If out of map, can't move
        if next_pos.left < 0 or next_pos.right > self.level.map.WIDTH \
                or next_pos.top < 0 or next_pos.bottom > self.level.map.HEIGHT:
            return False

        # If collide with terrains, can't move
        front_terrains = self.level.map.get_terrains_view(next_pos, self.direction)
        for terrain in front_terrains:
            if not terrain.is_passable() and terrain.rect.colliderect(next_pos):
                return False

        # If collide with other tanks, can't move
        #for tank in self.level.tanks:
            #if tank.state != Tank.S_DESTROYED and tank != self and tank.rect.colliderect(next_pos):
                #return False
        
        # Else move the tank
        self.rect.move_ip(dx, dy)

    def fire(self):
        #if self.direction == ActiveDrawable.DIR_UP or self.direction == Drawable.DIR_DOWN:
            #bullet_x = self.rect.center[0] - Bullet.SIZE_WIDTH / 2
            #bullet_y = self.rect.center[1] - Bullet.SIZE_HEIGHT / 2
        #else:
            #bullet_x = self.rect.center[0] - Bullet.SIZE_HEIGHT / 2
            #bullet_y = self.rect.center[1] - Bullet.SIZE_WIDTH / 2
        #bullet = Bullet(self.level, (bullet_x, bullet_y), speed=6, direction=self.direction, owner=self)
        #bullet.start()
        pass

    #def explode(self):
        #ex_center = self.rect.center
        #ex_pos = (ex_center[0] - Explosion.SIZE_WIDTH / 2, ex_center[1] - Explosion.SIZE_HEIGHT / 2)
        #ex = Explosion(ex_pos)
        #ex.start()

    #def hit(self, bullet):
        #self.health -= bullet.power
        #if self.health <= 0:
            #self.state = self.S_DESTROYED
            #self.explode()
            #self.destroy()


class PlayerTank(Tank): 

    yellow_tank_images = ActiveDrawable.construct_image_set(image.yellow_tank_player)
    green_tank_images = ActiveDrawable.construct_image_set(image.green_tank_player)

    YELLOW_PLAYER, GREEN_PLAYER = 1, 2

    def __init__(self, level, tankId, position, speed=.08, health=100, power=100, direction=ActiveDrawable.DIR_UP):
        startX = 26 * 5 if position == self.YELLOW_PLAYER else 26 * 10
        startY = 300
        image_set = self.yellow_tank_images if position == self.YELLOW_PLAYER else self.green_tank_images

        Tank.__init__(self, level, tankId, startX, startY, image_set, speed, health, power, direction)
        self.direction_requested = []

    def loop(self, time_passed): 
        for event in pygame.event.get():
            # TODO: Send these events to server for processing
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    self.firing = True
                elif event.key == pygame.K_UP:
                    self.direction_requested.append(self.DIR_UP)
                elif event.key == pygame.K_RIGHT:
                    self.direction_requested.append(self.DIR_RIGHT)
                elif event.key == pygame.K_DOWN:
                    self.direction_requested.append(self.DIR_DOWN)
                elif event.key == pygame.K_LEFT:
                    self.direction_requested.append(self.DIR_LEFT)

            elif event.type == pygame.KEYUP:
                if event.key == pygame.K_SPACE:
                    self.firing = False
                elif event.key == pygame.K_UP:
                    self.direction_requested.remove(self.DIR_UP)
                elif event.key == pygame.K_RIGHT:
                    self.direction_requested.remove(self.DIR_RIGHT)
                elif event.key == pygame.K_DOWN:
                    self.direction_requested.remove(self.DIR_DOWN)
                elif event.key == pygame.K_LEFT:
                    self.direction_requested.remove(self.DIR_LEFT)

        #if self.firing:
            #self.fire()
            #self.firing = False

        # Move player tank along the requested directions
        if len(self.direction_requested) > 0:
            self.move(self.direction_requested[0], time_passed)
            #if not self.sound_channel or not self.sound_channel.get_busy():
                #self.sound_channel = resource.sound_background.play()
        else:
            #self.sound_channel = None  # need to deallocate the channel
            pass
