#define SDL_MAIN_HANDLED
#include "debug.h"
#include "chip8.h"
#include "isa.h"
#include "hmi.h"
#include <SDL2/SDL.h>

// For usleep, works only for UNIX systems :(
#include <unistd.h>

unsigned short isTest = 0;
extern Chip8 g_chip8;

int loop = 1;

int main(int argc, char** argv){
	isTest = (argc == 1);
	
	if(isTest){
		writeMessage(0, "Expected usage: EMU ROM");
		writeMessage(1, "Reverting to test ROM...");
	}
	
	initCHIP();
	
	initHMI();
	
	loadROM(isTest ? "test/test_opcode.ch8" : argv[1]);
	
	while(loop){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) loop = 0;
			else if(e.type == SDL_KEYDOWN){
				for(int i = 0; i < KEY_NUMS; i++)
					if(e.key.keysym.sym == KEYPAD[i])
						g_chip8.keys[i] = 1;
			} else if(e.type == SDL_KEYUP){
				for(int i = 0; i < KEY_NUMS; i++)
					if(e.key.keysym.sym == KEYPAD[i])
						g_chip8.keys[i] = 0;
			}
		}
		
		cycle();
		if(g_chip8.flag_draw){
			g_chip8.flag_draw = 0;
			updateVideo();
		}
		usleep(1428); //700Hz
	}

	return 0;
}
