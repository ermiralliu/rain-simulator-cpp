#ifndef RAINDROP_HPP
#define RAINDROP_HPP

#include "SDL2/include/SDL_rect.h"
#include "RainType.hpp"

struct Raindrop{
    SDL_Rect dest;
    RainType type = SNOW;
};

#endif