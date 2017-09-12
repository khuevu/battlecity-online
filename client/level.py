from model.screen import GameScreen
from model.map import Map, Castle
from model.tank import PlayerTank, PartnerTank, EnemyTank
from model.stats import StatBar
import message


class Level(object): 

    def __init__(self, gameScreen, server, mapData, playerPosition): 
        self.server = server
        self.playerPosition = playerPosition
        # screen and engine should be private and updated through the add method
        self.scrn = gameScreen 
        # Level objects
        # Player tanks
        self.player = None
        self.partner = None
        # Enemy tanks
        self.enemies = []
        # Bullets
        self.bullets = []
        # Explosions
        self.explosions = []
        self._init_map(map_data=mapData)
        # Add the object to be drawn

    def _init_map(self, map_data):
        self.map = Map(self, map_data)
        self.add_to_screen(self.map)
        self.castle = Castle(self)
        self.add_to_screen(self.castle)
        self.stats = StatBar()
        self.add_to_screen(self.stats)

    def register_bullet(self, bullet):
        self.bullets.append(bullet)
        self.add_to_screen(bullet)

    def register_explosion(self, ex):
        self.explosions.append(ex)
        self.add_to_screen(ex)

    def register_enemy(self, enemy):
        self.enemies.append(enemy)
        self.add_to_screen(enemy)

    @property
    def tanks(self):
        all_tanks = []
        if self.player and not self.player.destroyed(): all_tanks.append(self.player)
        if self.partner and not self.partner.destroyed(): all_tanks.append(self.partner)
        all_tanks.extend(self.enemies)
        return all_tanks

    def add_to_screen(self, obj):
        self.scrn.add(obj)
        
    def loop(self, time_passed): 
        # Loop Player
        if self.player:
            self.player.loop(time_passed)

        # Loop Partner
        if self.partner: 
            self.partner.loop(time_passed)

        # Loop enemies and clear destroyed
        for e in self.enemies:
            e.loop(time_passed)
        n_enemey_killed = sum([1 for e in self.enemies if e.destroyed()])
        self.enemies = [e for e in self.enemies if not e.destroyed()]

        # Loop bullets and clear destroyed
        for b in self.bullets:
            b.loop(time_passed)
        self.bullets = [b for b in self.bullets if not b.destroyed()]

        # Loop explosions and clear destroyed
        for e in self.explosions:
            e.loop(time_passed)
        self.explosions = [e for e in self.explosions if not e.destroyed()]

        # process server update
        self._process_server_msg()

        # update stats
        self.stats.add_enemies_killed(n_enemey_killed)
        return True

    def _process_server_msg(self): 
        while True: 
            msg = self.server.get_message()
            if msg is not None: 
                msg_type, msg_data = msg

                if msg_type == message.TypeTankCreation: 
                    self._create_tank(msg_data)

                elif msg_type == message.TypeTankMovement:
                    self._update_tank_movement(msg_data)

                elif msg_type == message.TypeTankAction:
                    self._update_tank_action(msg_data)
                else:
                    print msg
            else: 
                break
        
    def _create_tank(self, data): 
        tank_id = data.id

        if tank_id <= 2: # create players' tanks
            print "Create player and partner tank"
            if tank_id == self.playerPosition:
                # create current player tank
                self.player = PlayerTank(self, tank_id, data.x, data.y)
                self.add_to_screen(self.player)
            else:
                self.partner = PartnerTank(self, tank_id, data.x, data.y)
                self.add_to_screen(self.partner)
        else: 
            print "Create enemy tanks of type %d" % data.type
            enemy_tank = EnemyTank(self, tank_id, tank_type=data.type,
                                   x=data.x, y=data.y, speed=data.speed, health=data.health,
                                   power=data.power, direction=data.direction)
            self.register_enemy(enemy_tank)

    def _is_partner_id(self, tank_id):
        # Player tank and partner tank has id either 1 or 2
        return tank_id <= 2 and tank_id != self.playerPosition

    def _get_tank_by_id(self, tank_id):
        if self._is_partner_id(tank_id):
            return self.partner
        else:
            for enemy in self.enemies:
                if enemy.id == tank_id:
                    return enemy
        return None

    def _update_tank_movement(self, data):
        tank_id = data.id
        tank = self._get_tank_by_id(tank_id)
        tank.update_movement(data.x, data.y, data.direction, data.moving)

    def _update_tank_action(self, data):
        tank_id = data.id
        tank = self._get_tank_by_id(tank_id)
        tank.update_action(data.action)

    def handle_user_input(self, event):
        self.player.handle_user_input(event)

