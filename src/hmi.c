#include "debug.h"
#include "chip8.h"
#include "hmi.h"
#include <SDL2/SDL.h>

#define CONTEXT_WIDTH 640
#define CONTEXT_HEIGHT 320

#define CONTEXT_SCALE CONTEXT_WIDTH/DISP_W

#define BG_COL 0, 0, 0, 255
#define FG_COL 255, 255, 255, 255

SDL_Window* w;
SDL_Renderer* r;

void initHMI(){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
		writeMessage(0, "SDL Initialisation Failed: %s", SDL_GetError());
		exit(-1);
	}
	SDL_CreateWindowAndRenderer(CONTEXT_WIDTH, CONTEXT_HEIGHT, SDL_WINDOW_SHOWN, &w, &r);
	
	if(!r | !w){
		writeMessage(0, "SDL Window and Renderer Creation Failed: %s", SDL_GetError());
		exit(-1);
	}
}

void updateVideo(){
	SDL_SetRenderDrawColor(r, BG_COL);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, FG_COL);
	SDL_Rect rect = (SDL_Rect){0, 0,
	CONTEXT_SCALE, CONTEXT_SCALE};
	for(int i=0;i<DISP_H;i++){
		rect.y = i*CONTEXT_SCALE;
		for(int j=0;j<DISP_W;j++){
			rect.x = j*CONTEXT_SCALE;
		if(g_chip8.gfx[(i*DISP_W)+j]) SDL_RenderFillRect(r, &rect);
		}
	}
	SDL_RenderPresent(r);
}