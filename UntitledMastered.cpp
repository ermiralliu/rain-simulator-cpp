#include <iostream>
#include <random>
#include <time.h>

#define SDL_MAIN_HANDLED
#include "SDL2/include/SDL.h"
#include "windowManagement.hpp"
#include "Raindrop.hpp"
#include "Textures.hpp"
#include "Atmosphere.hpp"

#define RAIN_NUMBER 3072			//total number of raindrops
#define FRAME_DELAY 16
#define RES_X 1280					//resolution in x dimension
#define RES_Y 720					//resolution in y dimension

void rain_init(Raindrop* drop, SDL_Rect& rainSource){
	std::subtract_with_carry_engine<unsigned int,11,7,11> engine((unsigned)time(NULL));
	for(int a=0; a<RAIN_NUMBER; ++a){
		drop[a].dest.x = 2+ engine()%1275;
		drop[a].dest.y = -RES_Y -20 + engine()%(RES_Y + 40);
		drop[a].dest.h = rainSource.h + 3*(engine()%6);
		drop[a].dest.w = rainSource.w*(drop[a].dest.h/rainSource.h);
	}
}
bool init(const char* title, int height, int width, int flags, SDL_Renderer*& renderer){	
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
		SDL_Window* g_pWindow = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,	height, width, flags);
		if(g_pWindow != 0){
			renderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		}
	}
	else 
		return false;
	return true;
}

void render(Raindrop* drop, SDL_Renderer* renderer, const Atmosphere& atmo, const Textures& texture){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if(atmo.type == HEATED_RAIN && atmo.background==0)
		SDL_RenderCopy(renderer, texture.backgroundTexture[5], &texture.backgroundSource, &texture.backgroundDest);
	else
		SDL_RenderCopy(renderer, texture.backgroundTexture[atmo.background], &texture.backgroundSource, &texture.backgroundDest);
	for(int a=0; a<RAIN_NUMBER; a++){
		if(drop[a].type == RAIN)
			SDL_RenderCopyEx(renderer, texture.rainTexture[0], &texture.rainSource, &drop[a].dest, atmo.angle, 0, SDL_FLIP_NONE);
		else if(drop[a].type == HAIL)
			SDL_RenderCopyEx(renderer, texture.rainTexture[1], &texture.rainSource, &drop[a].dest, atmo.angle, 0, SDL_FLIP_NONE);
		else if(drop[a].type == SNOW || drop[a].type == STATIC_SNOW)
			SDL_RenderCopy(renderer, texture.rainTexture[2], &texture.rainSource, &drop[a].dest);
	}
	SDL_RenderPresent(renderer);
}

void update(Uint32& reCnt, Raindrop* drop, const Atmosphere& atmo, unsigned int& background){
	std::subtract_with_carry_engine<unsigned int,8,7,11> engine((unsigned)time(NULL));
	for(int a=0 ; a<RAIN_NUMBER; a++){
		if(drop[a].type == RAIN || drop[a].type == HAIL || drop[a].type == SNOW){
			if(drop[a].dest.y< RES_Y +30){
				drop[a].dest.y += atmo.baseSpeed*drop[a].dest.h/20;
				if(drop[a].type == SNOW && drop[a].dest.y >= RES_Y-20 && drop[a].dest.y <= RES_Y-10 ){
					if(engine()%20==0 && background<4){
						drop[a].type = STATIC_SNOW;
						if(reCnt> 5000+ 5000*background)
							++background;
					}
					else if(background>=4)
						drop[a].type = STATIC_SNOW;
				}
				if(drop[a].type == SNOW)
					drop[a].dest.x -= atmo.wind*drop[a].dest.h/20 -1 + (engine()%3);	
				else if(drop[a].type == RAIN || drop[a].type == HAIL){
					drop[a].dest.x -= atmo.wind*drop[a].dest.h/20;
					if(background>0 &&  reCnt > 6000*background)
						--background;
				}
				if(drop[a].dest.x> RES_X)
					drop[a].dest.x = 2;
				else if(drop[a].dest.x < 1)
					drop[a].dest.x = RES_X;
			}
			else{
				reCnt++;
				drop[a].type = atmo.type;
				drop[a].dest.y = -5;
			}
		}
		else if(atmo.type != SNOW && drop[a].type == STATIC_SNOW){
			if(reCnt >= 70000)
				drop[a].type = atmo.type;
			else if(engine()%256 == 0)
					drop[a].type = atmo.type;
		}
	}
}

void event(Uint32& count, Uint32& reCount, bool& running, SDL_Renderer* renderer, Atmosphere& atmo, const Textures& texture){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if (e.type== SDL_QUIT){
			count+=reCount;
			running = Quit(count);
		}	
		else if(e.type == SDL_KEYDOWN) {
			if(e.key.keysym.sym == SDLK_ESCAPE){
				count+= reCount;
				running = Quit(count);
			}		
			else if(e.key.keysym.sym == SDLK_w){
					showConsole();
					std::cout<<"Enter wind speed (pixel per second), positive values to the right:\n";
					double wind;
					std::cin>>wind;
					atmo.updateWind(wind/60);
					exeToForeground();
			}
   			else if(e.key.keysym.sym == SDLK_t){
					showConsole();
					count += reCount;
					reCount = 0;
					std::cout<<"Enter temperature: (in degree Celsius)\n";
					short int temperature;
					std::cin>> temperature;
					while(temperature> 200 || temperature< -274){
						if(temperature> 200){
							SDL_RenderClear(renderer);
							SDL_RenderCopy(renderer, texture.backgroundTexture[5], &texture.backgroundSource, &texture.backgroundDest);
							SDL_RenderPresent(renderer);
							std::cout<<"\nTOO HOT\nAll the rain has vaporized\nEnter another temperature (less than 200):\n";
						}	
						else if(temperature< -274){
							SDL_RenderClear(renderer);
							SDL_RenderPresent(renderer);
							std::cout<<"\nUnder absolute 0\nNo Sun, No hope, Nothing\nReenter another temperature (more than -274):\n";
						}	
						std::cin>>temperature;
					}
					atmo.arrange(temperature);
					exeToForeground();
			}
			else if(e.key.keysym.sym == SDLK_r){
					showConsole();
					std::cout<<"Number of fallen raindrops fallen until now is:"<< count+reCount
					<<"\nPress enter to continue:\n";
					std::cin.get();
					exeToForeground();
			}
		}
	}
}

int main(int argc, char** argv){
	std::ios_base::sync_with_stdio(false);
	showConsole(false);
	bool running;
	SDL_Renderer* renderer;
	
	if(init("Rain Simulator", RES_X, RES_Y, SDL_WINDOW_SHOWN| SDL_WINDOW_ALLOW_HIGHDPI, renderer))
		running = true;
	else
		return 1;
	
	Textures text;
	Atmosphere atmo;
	text.loadTextures(renderer);
	
	Raindrop drop[RAIN_NUMBER];
	rain_init(drop, text.rainSource);
	Uint32 reCount = 0;
	Uint32 count = 0;
	
	while(running){
		Uint32 frameStart = SDL_GetTicks();
		update(reCount, drop, atmo, atmo.background);
		render(drop, renderer, atmo, text);
		int frameTime = SDL_GetTicks() - frameStart;
		if(FRAME_DELAY > frameTime)
			SDL_Delay(FRAME_DELAY - frameTime);
		event(count, reCount, running, renderer, atmo, text); //some of these are passed by reference and some are const references
	}
	SDL_Quit();
	return 0;
}