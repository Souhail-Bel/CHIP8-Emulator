#include "debug.h"
#include "chip8.h"
#include "isa.h"
#include "hmi.h"

// For usleep, works only for UNIX systems :(
#include <unistd.h>

unsigned short isTest = 0;
extern Chip8 g_chip8;

int main(int argc, char** argv){
	isTest = argc == 1;
	
	if(isTest){
		writeMessage(0, "Expected usage: EMU ROM");
		writeMessage(1, "Reverting to test ROM...");
	}
	
	initCHIP();
	
	loadROM(isTest ? "test/test_opcode.ch8" : argv[1]);
	
	while(1){
		cycle();
		if(g_chip8.flag_draw){
			g_chip8.flag_draw = 0;
			draw(g_chip8.gfx);
		}
		usleep(1428); //700Hz
	}
	
	return 0;
}