import pygame
import os


pygame.mixer.init(frequency=44100, size=-16, channels=1, buffer=1024)
_sound_dir = os.path.dirname(os.path.realpath(__file__))

gamestart = pygame.mixer.Sound(_sound_dir + '/gamestart.ogg')
gameend = pygame.mixer.Sound(_sound_dir + '/gameover.ogg')
fire = pygame.mixer.Sound(_sound_dir + '/fire.ogg')
explosion = pygame.mixer.Sound(_sound_dir + '/explosion.ogg')
moving = pygame.mixer.Sound(_sound_dir + '/background.ogg')
brick = pygame.mixer.Sound(_sound_dir + '/brick.ogg')
steel = pygame.mixer.Sound(_sound_dir + '/steel.ogg')
