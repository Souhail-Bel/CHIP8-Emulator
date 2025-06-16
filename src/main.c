#include "debug.h"
#include "chip8.h"
#include "isa.h"
#include <SDL2/SDL.h>

// For usleep, works only for UNIX systems :(
#include <unistd.h>

unsigned short isTest = 0;
extern Chip8 g_chip8;

#define BG_COL 0, 0, 0, 255
#define FG_COL 255, 255, 255, 255

SDL_Window* w;
SDL_Renderer* r;

int main(int argc, char** argv){
	isTest = argc == 1;
	
	if(isTest){
		writeMessage(0, "Expected usage: EMU ROM");
		writeMessage(1, "Reverting to test ROM...");
	}

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640, 320, 0, &w, &r);
	
	initCHIP();
	
	loadROM(isTest ? "test/test_opcode.ch8" : argv[1]);
	
  int quit = 0;
  while(!quit){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){
        quit = 1;
      }
    }
    SDL_SetRenderDrawColor(r, BG_COL);
    SDL_RenderClear(r);
		cycle();
    SDL_SetRenderDrawColor(r, FG_COL);
    SDL_Rect rect = (SDL_Rect){0, 0, 10, 10};
    for(int i=0;i<32;i++){
      rect.y = i*10;
      for(int j=0;j<64;j++){
        rect.x = j*10;
        if(g_chip8.gfx[i*64+j])
          SDL_RenderFillRect(r, &rect);
      }
    }
		usleep(1428); //700Hz

    SDL_RenderPresent(r);
  }

	return 0;
}
