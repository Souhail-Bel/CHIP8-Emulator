#define SDL_MAIN_HANDLED
#include "debug.h"
#include "chip8.h"
#include "isa.h"
#include "hmi.h"


#include <SDL2/SDL.h>
#include <string.h>
// For usleep, works only for UNIX systems :(
#include <unistd.h>

unsigned short isTest = 0;
extern Chip8 g_chip8;
extern unsigned short loop;

int main(int argc, char** argv){
	initCHIP();
	
	isTest = (argc == 1);
	
	if(isTest){
		writeMessage(0, "Expected usage: EMU ROM");
		writeMessage(1, "Reverting to test ROM...");
	}
	
	loadROM(isTest ? "test/test_opcode.ch8" : argv[1]);
	
	
	initHMI();
	
	loop = 1;
	
	while(loop){
		cycle();
		
		inputHandler();
		
		if(g_chip8.flag_sound){
			g_chip8.flag_sound = 0;
			beep();
		}
		
		if(g_chip8.flag_draw){
			g_chip8.flag_draw = 0;
			updateVideo();
		}
		usleep(1428); //700Hz
	}
	
	closeSDL();
	dumpSTATE();

	return 0;
}
