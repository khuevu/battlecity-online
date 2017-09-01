import pygame
import os

_image_dir = os.path.dirname(os.path.realpath(__file__))

# All sprites
sprites = pygame.transform.scale(pygame.image.load(_image_dir + "/sprites.gif"), [192, 224])

game_icon = sprites.subsurface(0, 0, 26, 26)

yellow_tank_player = sprites.subsurface(0, 0, 26, 26)
green_tank_player = sprites.subsurface(32, 0, 26, 26)

tank_enemy_imgs = [
    sprites.subsurface(32*2, 0, 13*2, 15*2),
    sprites.subsurface(48*2, 0, 13*2, 15*2),
    sprites.subsurface(64*2, 0, 13*2, 15*2),
    sprites.subsurface(80*2, 0, 13*2, 15*2),
    ]
tank_enemy_with_bonus_imgs = [
    sprites.subsurface(32*2, 16*2, 13*2, 15*2),
    sprites.subsurface(48*2, 16*2, 13*2, 15*2),
    sprites.subsurface(64*2, 16*2, 13*2, 15*2),
    sprites.subsurface(80*2, 16*2, 13*2, 15*2)
    ]

# Spawning Lights
spawning_light_imgs = [
    sprites.subsurface(64, 96, 32, 32),
    sprites.subsurface(96, 96, 32, 32)
    ]

# Player Cloak
invincible_cloak_imgs = [
    sprites.subsurface(0, 96, 32, 32),
    sprites.subsurface(32, 96, 32, 32)
]

# Side Bar
tank_icon_enemy = sprites.subsurface(160, 112, 16, 16)
tank_icon_player = sprites.subsurface(160 + 16, 112, 16, 16)

# Castle
castle_img = sprites.subsurface(0, 15*2, 16*2, 16*2)
castle_destroyed_img = sprites.subsurface(16*2, 15*2, 16*2, 16*2)

# Bonuses

# Bullet
bullet_img = sprites.subsurface(75*2, 74*2, 3*2, 4*2)

explosion_imgs = [
    sprites.subsurface(0, 80*2, 32*2, 32*2),
    sprites.subsurface(32*2, 80*2, 32*2, 32*2),
    sprites.subsurface(64*2, 80*2, 32*2, 32*2)
]

# Tile
#tile_empty = pygame.Surface((16, 16))
tile_brick = sprites.subsurface(48*2, 64*2, 16, 16)
tile_steel = sprites.subsurface(48*2, 72*2, 16, 16)
tile_grass = sprites.subsurface(56*2, 72*2, 16, 16)
tile_water = sprites.subsurface(64*2, 64*2, 16, 16)
tile_froze = sprites.subsurface(64*2, 72*2, 8*2, 8*2)
