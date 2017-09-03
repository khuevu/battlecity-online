import image
import sound
from model import Drawable, ActiveDrawable
from model.bullet import Bullet
from model.explosion import explode
import pygame
import message
from util import currenttime_millis


class Tank(ActiveDrawable):

    Z = 100
    SIZE = 26
    ACTION_FIRE, ACTION_EXPLODE, ACTION_ITEM = range(3)

    def __init__(self, level, tankId, x, y, images, speed, 
            health, power, direction=ActiveDrawable.DIR_UP, recharge_time=500):
        ActiveDrawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), images, speed, direction)
        self.id = tankId
        self.level = level
        self.health = health
        self.power = power

        self.stopped = True
        # Recharge time in milliseconds
        self.recharge_time = recharge_time
        self.last_fired = 0

        # cloak
        self.cloak = None

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
            if self != tank and not tank.destroyed() and tank.collide(next_pos):
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
        """ Trigger fire. Return True if fire successfully and False otherwise.
        Tank can only fire after it has fully recharged
        """
        current_time = currenttime_millis()
        if current_time - self.last_fired < self.recharge_time: # not fully recharge yet
            return False

        tank_head_x, tank_head_y = self._get_tank_head()
        if self.direction == ActiveDrawable.DIR_UP or self.direction == ActiveDrawable.DIR_DOWN:
            bullet_x = tank_head_x - Bullet.SIZE_WIDTH / 2
            bullet_y = tank_head_y - Bullet.SIZE_HEIGHT / 2
        else:
            bullet_x = tank_head_x - Bullet.SIZE_HEIGHT / 2
            bullet_y = tank_head_y - Bullet.SIZE_WIDTH / 2
        bullet = Bullet(self.level, bullet_x, bullet_y, direction=self.direction, owner=self, power=self.power)
        self.level.register_bullet(bullet)
        # Reset time
        self.last_fired = current_time
        # Success
        sound.fire.play()
        return True

    def update_movement(self, x, y, direction, moving):
        self.x = x
        self.y = y
        self.rotate(direction)
        if moving:
            self.stopped = False
        else:
            self.stopped = True

    def update_action(self, action):
        if action == self.ACTION_FIRE:
            self.fire()
        elif action == self.ACTION_EXPLODE:
            print "Update explode action from server for tank ", self.id
            self.destroy()
            self.level.register_explosion(explode(self))
        else:
            # TODO: process other actions
            pass

    def _send_explode_action(self):
        print "Send tank explode action for tank ", self.id
        server = self.level.server
        tank_action = message.MsgTankAction(self.id, self.ACTION_EXPLODE)
        server.send_message(message.TypeTankAction, tank_action)

    def hit(self, bullet):
        if self.is_cloaked():
            # When a tank is cloaked, it is not hitable.
            return

        self.health -= bullet.power
        if self.health <= 0:
            self.destroy()
            self.level.register_explosion(explode(self))

            # update server of tank destruction
            self._send_explode_action()

    def cloak_on(self):
        self.cloak = InvincibleCloak(self.x, self.y)
        self.level.scrn.add(self.cloak)

    def is_cloaked(self):
        return self.cloak and not self.cloak.destroyed()

    def loop(self, time_passed):
        if self.is_cloaked():
            # move cloak along the tank
            self.cloak.x = self.x
            self.cloak.y = self.y
            self.cloak.loop(time_passed)


YELLOW_PLAYER, GREEN_PLAYER = 1, 2

def get_player_tank_images(position): 
    if position == YELLOW_PLAYER:
        image_set = ActiveDrawable.construct_image_set(image.yellow_tank_player) 
    else:
        image_set = ActiveDrawable.construct_image_set(image.green_tank_player)
    return image_set


class InvincibleCloak(Drawable):

    Z = 150
    SIZE = 32
    FRAME_DUR = 90
    LIFE_TIME = 20 * 1000

    def __init__(self, x, y):
        self.duration = 0
        Drawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), image.invincible_cloak_imgs[0])

    def loop(self, time_passed):
        self.duration += time_passed
        state = self.duration / self.FRAME_DUR % 2 # only change state after ANIM_FRAME_DUR
        self.image = image.invincible_cloak_imgs[state]
        if self.duration > self.LIFE_TIME:
            self.destroy()


class PartnerTank(Tank):

    def __init__(self, level, play_position, x, y, speed=.08, health=100, power=100, direction=ActiveDrawable.DIR_UP):
        image_set = get_player_tank_images(play_position)
        tankId = play_position
        print "Initialize PartnerTank at {}-{}".format(x, y)
        Tank.__init__(self, level, tankId, x, y, image_set, speed, health, power, direction)
        self.cloak_on()

    def loop(self, time_passed):
        """ Moving the partner tank along the current direction
        This is an optimization. Updates are only received when partner tank change direction | action
        instead of continuous updates, therefore, reducing the amount of data communicated over the network
        """
        Tank.loop(self, time_passed)
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
        self.firing_requested = False
        self.cloak_on()
        self.moving_sound = None

    def _send_movement_update(self, moving):
        server = self.level.server
        tank_movement = message.MsgTankMovement(self.id, self.x, self.y, self.direction, moving)
        print "Send update player tank position: [{}: {}-{} -> direcition: {}]".format(self.id, self.x, self.y, self.direction)
        server.send_message(message.TypeTankMovement, tank_movement)

    def _send_fire_action(self):
        server = self.level.server
        tank_action = message.MsgTankAction(self.id, self.ACTION_FIRE)
        server.send_message(message.TypeTankAction, tank_action)

    def loop(self, time_passed):
        Tank.loop(self, time_passed)

        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    self.firing_requested = True
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
                    self.firing_requested = False
                elif event.key == pygame.K_UP:
                    self.direction_requested.remove(self.DIR_UP)
                elif event.key == pygame.K_RIGHT:
                    self.direction_requested.remove(self.DIR_RIGHT)
                elif event.key == pygame.K_DOWN:
                    self.direction_requested.remove(self.DIR_DOWN)
                elif event.key == pygame.K_LEFT:
                    self.direction_requested.remove(self.DIR_LEFT)

        if self.destroyed():
            return

        if self.firing_requested:
            fired = self.fire()
            if fired:
                self._send_fire_action()
            self.firing_requested = False

        # Move player tank along the requested directions
        if len(self.direction_requested) > 0:
            old_direction = self.direction
            moved = self.move(self.direction_requested[0], time_passed)
            # only update if direction or state changed, i.e, tank from stopping to moving
            if old_direction != self.direction or (self.stopped and moved):
                self._send_movement_update(moving=True)

            self.stopped = not moved
            if not self.moving_sound or not self.moving_sound.get_busy():
                self.moving_sound = sound.moving.play()
        else:
            self.moving_sound = None  # need to deallocate the channel
            if not self.stopped:
                self.stopped = True
                # send update that the tank is stopped
                self._send_movement_update(moving=False)


class SpawningLight(Drawable):

    Z = 150
    SIZE = 32
    FRAME_DUR = 90

    def __init__(self, x, y):
        self.duration = 0
        Drawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), image.spawning_light_imgs[0])

    def loop(self, time_passed):
        self.duration += time_passed
        state = self.duration / self.FRAME_DUR % 2 # only change state after ANIM_FRAME_DUR
        self.image = image.spawning_light_imgs[state]


class EnemyTank(Tank):

    TYPE_BASIC, TYPE_FAST, TYPE_POWER, TYPE_ARMOR = range(4)

    TYPE_IMAGES = {
        TYPE_BASIC: image.tank_enemy_imgs[0],
        TYPE_FAST: image.tank_enemy_imgs[1],
        TYPE_POWER: image.tank_enemy_imgs[2],
        TYPE_ARMOR: image.tank_enemy_imgs[3]
    }

    def _spawn(self):
        # Hide the tank when it is spawning
        self.hide()
        # Display the spawning light
        self.spawn_light = SpawningLight(self.x, self.y)
        self.level.scrn.add(self.spawn_light)

    def is_spawning(self):
        return self.spawn_light and not self.spawn_light.destroyed()

    def __init__(self, level, tank_id, tank_type, x, y, speed=.08, health=100, power=100, direction=ActiveDrawable.DIR_UP):
        image_set = ActiveDrawable.construct_image_set(self.TYPE_IMAGES[tank_type])
        Tank.__init__(self, level, tank_id, x, y, image_set, speed, health, power, direction)
        self._spawn()

    def loop(self, time_passed):
        if self.is_spawning():
            self.spawn_light.loop(time_passed)

        elif not self.stopped:
            self.move(self.direction, time_passed)

    def update_movement(self, x, y, direction, moving):
        if self.is_spawning():
            # Activate tank and remove the spawning light
            self.spawn_light.destroy()
            self.spawn_light = None
            self.activate()

        Tank.update_movement(self, x, y, direction, moving)

    def hit(self, bullet):
        self.health -= bullet.power
        if self.health <= 0:
            self._send_explode_action()