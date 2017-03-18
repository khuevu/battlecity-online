import image
from model import Drawable, ActiveDrawable
from model.bullet import Bullet
import pygame
import message


class Tank(ActiveDrawable):

    Z = 100
    SIZE = 26
    ACTION_STOP, ACTION_MOVE, ACTION_FIRE = range(3)

    def __init__(self, level, tankId, x, y, images, speed, 
            health, power, direction=ActiveDrawable.DIR_UP):
        ActiveDrawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), images, speed, direction)
        self.id = tankId
        self.level = level
        self.health = health
        self.power = power
        #self.level.add_to_screen(self)
        self.stopped = True

    def move(self, direction, time_passed):
        self.rotate(direction) 
        # Check if the next position in the current direction will lead to collisions
        next_pos, dx, dy = self.calc_next_pos(time_passed)
        # If out of map, can't move
        if not self.level.map.is_within(next_pos):
            return False

        # If collide with terrains, can't move
        if self.level.map.get_block(next_pos, self.direction):
            return False

        # If collide with other tanks, can't move
        for tank in self.level.tanks:
            if self != tank and self.collide(tank):
                return False

        # Else move the tank
        self.do_move(dx, dy)
        return True

    def _get_tank_head(self):
        """ Get the coordinate of the gun head of the tank """
        if self.direction == ActiveDrawable.DIR_UP:
            x = self.x + self.SIZE / 2
            y = self.y

        elif self.direction == ActiveDrawable.DIR_DOWN:
            x = self.x + self.SIZE / 2
            y = self.y + self.SIZE

        elif self.direction == ActiveDrawable.DIR_LEFT:
            x = self.x
            y = self.y + self.SIZE / 2

        else:
            x = self.x + self.SIZE
            y = self.y + self.SIZE / 2
        return x, y

    def fire(self):
        tank_head_x, tank_head_y = self._get_tank_head()
        if self.direction == ActiveDrawable.DIR_UP or self.direction == ActiveDrawable.DIR_DOWN:
            bullet_x = tank_head_x - Bullet.SIZE_WIDTH / 2
            bullet_y = tank_head_y - Bullet.SIZE_HEIGHT / 2
        else:
            bullet_x = tank_head_x - Bullet.SIZE_HEIGHT / 2
            bullet_y = tank_head_y - Bullet.SIZE_WIDTH / 2
        bullet = Bullet(self.level, bullet_x, bullet_y, direction=self.direction, owner=self, power=self.power)
        self.level.register_bullet(bullet)

    def update(self, x, y, direction, action): 
        self.x = x
        self.y = y
        self.rotate(direction)
        if action == self.ACTION_STOP:
            self.stopped = True
        elif action == self.ACTION_MOVE:
            self.stopped = False
        elif action == self.ACTION_FIRE:
            self.fire()
        else:
            print "Invalid action", action

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


YELLOW_PLAYER, GREEN_PLAYER = 1, 2

def get_player_tank_images(position): 
    if position == YELLOW_PLAYER:
        image_set = ActiveDrawable.construct_image_set(image.yellow_tank_player) 
    else:
        image_set = ActiveDrawable.construct_image_set(image.green_tank_player)
    return image_set


class PartnerTank(Tank): 

    def __init__(self, level, play_position, x, y, speed=.08, health=100, power=100, direction=ActiveDrawable.DIR_UP):
        image_set = get_player_tank_images(play_position)
        tankId = play_position
        print "Initialize PartnerTank at {}-{}".format(x, y)
        Tank.__init__(self, level, tankId, x, y, image_set, speed, health, power, direction)

    def loop(self, time_passed):
        """ Moving the partner tank along the current direction
        This is an optimization. Updates are only received when partner tank change direction | action
        instead of continuous updates, therefore, reducing the amount of data communicated over the network
        """
        if not self.stopped:
            self.move(self.direction, time_passed)


class PlayerTank(Tank): 

    def __init__(self, level, play_position, x, y, speed=.08, health=100, power=100, direction=ActiveDrawable.DIR_UP):
        image_set = get_player_tank_images(play_position)
        tankId = play_position
        print "Initialize PlayerTank at {}-{}".format(x, y)
        Tank.__init__(self, level, tankId, x, y, image_set, speed, health, power, direction)
        # control parameters
        self.direction_requested = []
        self.firing = False

    def _send_action_update(self, action): 
        server = self.level.server
        # only send position update if the direction change
        tankPos = message.MsgTankMovement(self.id, self.x, self.y, self.direction, action)
        print "Send update player tank position: [{}: {}-{} -> direcition: {}]".format(self.id, self.x, self.y, self.direction)
        server.send_message(message.TypeTankMovement, tankPos)

    def loop(self, time_passed): 
        for event in pygame.event.get():
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

        if self.firing:
            self.fire()
            self.firing = False

        # Move player tank along the requested directions
        if len(self.direction_requested) > 0:
            old_direction = self.direction
            moved = self.move(self.direction_requested[0], time_passed)
            # only update if direction or state changed, i.e, tank from stopping to moving
            if old_direction != self.direction or (self.stopped and moved):
                self._send_action_update(action=self.ACTION_MOVE)

            self.stopped = not moved
            #if not self.sound_channel or not self.sound_channel.get_busy():
                #self.sound_channel = resource.sound_background.play()
        else:
            #self.sound_channel = None  # need to deallocate the channel
            if not self.stopped:
                self.stopped = True
                # send update that the tank is stopped
                self._send_action_update(action=self.ACTION_STOP)
