#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include "SDL2/include/SDL_rect.h"
#include "SDL2/include/SDL_render.h"


class Textures{
    private:
        void loadRainTexture(SDL_Renderer* renderer);
        void loadBackgrounds(SDL_Renderer* renderer);

    public:
        //should be private but idc
        Textures(){};
        SDL_Texture* rainTexture[3];
        SDL_Texture* backgroundTexture[6];
        SDL_Rect rainSource;
        SDL_Rect backgroundSource;
        SDL_Rect backgroundDest;
    
        Textures(const Textures& obj) = delete;
        void loadTextures(SDL_Renderer* renderer);
        static Textures* createTextures();
        static SDL_Texture* TextureLoader(const char* texture, SDL_Renderer* ren);
};

#endif