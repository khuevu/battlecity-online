import pygame
import os


_font_dir = os.path.dirname(os.path.realpath(__file__))
_cache_fonts = dict()


def with_size(font_size):
    if font_size in _cache_fonts: 
        return _cache_fonts[font_size] 
    else: 
        f = pygame.font.Font(_font_dir + "/prstart.ttf", font_size)
        _cache_fonts[font_size] = f
        return f

#small_font = pygame.font.Font(_font_dir + "/prstart.ttf", 16)
#large_font = pygame.font.Font(_font_dir + "/prstart.ttf", 34)
