import pygame
import os


pygame.mixer.init(frequency=44100, size=-16, channels=1, buffer=1024)
_sound_dir = os.path.dirname(os.path.realpath(__file__))

sound_gamestart = pygame.mixer.Sound(_sound_dir + '/gamestart.ogg')
sound_gameend = pygame.mixer.Sound(_sound_dir + '/gameover.ogg')
sound_background = pygame.mixer.Sound(_sound_dir + '/background.ogg')
sound_fire = pygame.mixer.Sound(_sound_dir + '/fire.ogg')
sound_explosion = pygame.mixer.Sound(_sound_dir + '/explosion.ogg')
sound_brick = pygame.mixer.Sound(_sound_dir + '/brick.ogg')
sound_steel = pygame.mixer.Sound(_sound_dir + '/steel.ogg')
