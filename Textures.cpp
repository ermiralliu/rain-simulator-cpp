#include "Textures.hpp"
#include "RainType.hpp"
#include "SDL2/include/SDL_image.h"

void Textures::loadRainTexture(SDL_Renderer* renderer){
    rainTexture[RAIN] = TextureLoader("assets/raindrop.png", renderer);
	rainTexture[HAIL] = TextureLoader("assets/hail.png", renderer);
	rainTexture[SNOW] = TextureLoader("assets/snow.png", renderer);
	SDL_QueryTexture(rainTexture[RAIN], NULL, NULL, &rainSource.w, &rainSource.h);
	rainSource.x = 0;
	rainSource.y = 0;
}
void Textures::loadBackgrounds(SDL_Renderer* renderer){
    backgroundTexture[0] = TextureLoader("assets/sky1.png", renderer);
	backgroundTexture[1] = TextureLoader("assets/sky2.png", renderer);
	backgroundTexture[2] = TextureLoader("assets/sky3.png", renderer);
	backgroundTexture[3] = TextureLoader("assets/sky4.png", renderer);
	backgroundTexture[4] = TextureLoader("assets/sky5.png", renderer);
	backgroundTexture[5] = TextureLoader("assets/skyHot.png", renderer);
	SDL_QueryTexture(backgroundTexture[0], NULL, NULL, &backgroundSource.w, &backgroundSource.h);
	backgroundDest.x = 0; backgroundSource.x = 0;
	backgroundDest.y = 0; backgroundSource.y = 0;
	backgroundDest.w = backgroundSource.w;
	backgroundDest.h = backgroundSource.h;
}

void Textures::loadTextures(SDL_Renderer* renderer){
    loadRainTexture(renderer);
    loadBackgrounds(renderer);
}

SDL_Texture* Textures::TextureLoader(const char* texture, SDL_Renderer* ren){
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}