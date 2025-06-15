#include "chip8.h"
#include "hmi.h"

#include <stdio.h>
#include <stdlib.h>

void draw(unsigned char* gfx){
	#ifndef DEBUG_MODE
	system("clear");
	#endif
	for(int y = 0; y < DISP_H; y++){
		for(int x = 0; x < DISP_W; x++){
			putchar(gfx[(y*DISP_W) + x] ? '#' : ' ');
		}
		putchar('\n');
	}
}