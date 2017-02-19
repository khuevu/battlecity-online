from model import Drawable
import image
import pygame


class Terrain(Drawable):

    # Types
    BRICK, STONE, BUSH, WATER, ICE = range(5)

    SIZE = 16

    tiles = [image.tile_brick, image.tile_steel, 
            image.tile_grass, image.tile_water, image.tile_froze]

    @staticmethod
    def make(symbol, x, y): 
        t_type = Terrain._get_type(symbol)
        if t_type is not None: 
            return Terrain(t_type, x, y)
        else:
            return None

    @staticmethod
    def _get_type(tile_symbol):
        if tile_symbol == "#":
            return Terrain.BRICK
        elif tile_symbol == "@":
            return Terrain.STONE
        elif tile_symbol == "~":
            return Terrain.WATER
        elif tile_symbol == "%":
            return Terrain.BUSH
        elif tile_symbol == "-":
            return Terrain.ICE
        else:
            return None

    def __init__(self, t_type, x, y):
        self.type = t_type
        t_image = self.tiles[t_type]
        Drawable.__init__(self, pygame.Rect((x, y), (self.SIZE, self.SIZE)), t_image)

    def is_collideable(self):
        return True if self.type == Terrain.BRICK or self.type == Terrain.STONE else False

    def is_passable(self):
        return False if self.is_collideable() or self.type == Terrain.WATER else True
    
    #def hit(self, bullet):
        #if not self.is_collideable():
            #return
        #ex_center = bullet.rect.center
        #ex_pos = (ex_center[0] - Explosion.SIZE_WIDTH / 2, ex_center[1] - Explosion.SIZE_HEIGHT / 2)
        #ex_sound = resource.sound_brick if self.type == self.BRICK else resource.sound_steel
        #ex = Explosion(ex_pos, sound=ex_sound)
        #ex.start()
        ## destroy the terrain if the bullet is powerful enough
        #if bullet.power > 100 or self.type == self.BRICK:
            #self.type = self.EMPTY
            #self.image = self.tiles[self.type]
            #self.destroy()

    #def draw(self, screen):
        #if self.type == self.EMPTY:
            #return
        #else:
            #Drawable.draw(self, screen)


#class Castle(Drawable):

    #SIZE = 32

    #def __init__(self, level):
        #x = 12 * 16
        #y = 24 * 16
        #self.level = level
        #Drawable.__init__(self, (x, y), self.SIZE, self.SIZE, image=resource.castle_img)

    #def hit(self, bullet):
        #ex_pos = self.rect.topleft
        #ex = Explosion(ex_pos, extra=True)
        #ex.start()
        #self.image = resource.castle_destroyed_img
        #self.level.on_castle_destroyed()


class Map(Drawable):

    Z = 200
    WIDTH = 26 * Terrain.SIZE
    HEIGHT = 26 * Terrain.SIZE
    DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT = range(4)

    def __init__(self, map_data):
        Drawable.__init__(self, None, None) # Map itself doesn't have image
        self.terrains = []
        x, y = 0, 0
        row = []

        for s in map_data:
            row.append(Terrain.make(s, x, y))

            x += Terrain.SIZE
            if (x == self.WIDTH): 
                # move to next row
                self.terrains.append(row)
                row = []
                y += Terrain.SIZE
                x = 0 
        assert self.HEIGHT == y
        
    def get_terrain(self, pos):
        return self.terrains[pos[1] / Terrain.SIZE][pos[0] / Terrain.SIZE]

    def get_terrains_view(self, pos_rect, direction):
        # get all overlap terrains
        if direction == self.DIR_UP:
            start_x, start_y = pos_rect.topleft
            end_y = start_y
            end_x = start_x + pos_rect.width

        elif direction == self.DIR_LEFT:
            start_x, start_y = pos_rect.topleft
            end_x = start_x
            end_y = start_y + pos_rect.height

        elif direction == self.DIR_RIGHT:
            start_x, start_y = pos_rect.topright
            end_x = start_x
            end_y = start_y + pos_rect.height

        elif direction == self.DIR_DOWN:
            start_x, start_y = pos_rect.bottomleft
            end_y = start_y
            end_x = start_x + pos_rect.width

        # Get the index range of terrains that in front of the input rect
        start_x_index = start_x / Terrain.SIZE
        end_x_index = min((end_x + Terrain.SIZE) / Terrain.SIZE, self.WIDTH / Terrain.SIZE)

        start_y_index = start_y / Terrain.SIZE
        end_y_index = min((end_y + Terrain.SIZE) / Terrain.SIZE, self.HEIGHT / Terrain.SIZE)

        view = []
        for col in xrange(start_x_index, end_x_index):
            for row in xrange(start_y_index, end_y_index):
                if self.terrains[row][col]:
                    view.append(self.terrains[row][col])
        return view

    def draw(self, screen): 
        for row in self.terrains: 
            for t in row: 
                if t: # if not an empty terrain 
                    t.draw(screen)