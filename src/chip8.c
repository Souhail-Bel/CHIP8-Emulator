#include "debug.h"
#include "chip8.h"
#include "isa.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

Chip8 g_chip8;

void initCHIP(void){
	g_chip8 = (Chip8) {.PC = PROGRAM_OFFSET};
	memcpy(g_chip8.memory + FONT_OFFSET, font, FONT_SIZE);
	srand(time(NULL));
	init_opcode_tables();
	DEBUG_PRINT("CPU is READY");
}

void loadROM(const char* rom){
	FILE* f = fopen(rom, "rb");
	if(!f){
		writeMessage(0, "ROM not found.");
		exit(-1);
	}
	
	size_t freeSpace = sizeof(g_chip8.memory) - PROGRAM_OFFSET;
	
	fseek(f, 0, SEEK_END);
	size_t fileSize = ftell(f);
	rewind(f);
	if(fileSize > freeSpace){
		writeMessage(0, "Not enough memory space for ROM.");
		fclose(f);
		exit(-1);
	}
	
	if(fread(g_chip8.memory + PROGRAM_OFFSET, 1, freeSpace, f) != fileSize){
		writeMessage(0, "Unable to read ROM file.");
		fclose(f);
		exit(-1);
	}
	fclose(f);
	
	DEBUG_PRINT("Successfully loaded ROM: %s", rom);
}

void dumpSTATE(void){
	const char* fileName = "mem_dump.bin";
	FILE *f = NULL;
	
	f = fopen(fileName, "wb");
	
	if(!f){
		writeMessage(0, "Unable to open memory dump file for writing.");
		exit(-1);
	}
	
	size_t elemsW = fwrite(g_chip8.memory, sizeof(uint8_t), MEMORY_SIZE, f);
	
	if(elemsW != MEMORY_SIZE){
		writeMessage(0, "Bytes %zu/%d were written to %s", elemsW, MEMORY_SIZE, f);
		fclose(f);
		exit(-1);
	}
	
	writeMessage(2, "Memory dumped successfully.");
	
	fclose(f);
}

void updateTimers(void) {
    // Update timers at around 60 Hz
    static int timer_counter = 0;
    const int cycles_per_timer_update = 12; // 700 Hz / 60 Hz = 11.67
    timer_counter++;
    if(timer_counter >= cycles_per_timer_update){
        timer_counter = 0;
        if(g_chip8.timer_delay > 0) g_chip8.timer_delay--;
        if(g_chip8.timer_sound > 0){
            g_chip8.flag_sound = 1;
            g_chip8.timer_sound--;
        }
    }
}

void cycle(void){
	// Reset flag
	//g_chip8.flag_sound = 0;
	
	// Fetch
	word op = (g_chip8.memory[g_chip8.PC]) << 8;
	op |= g_chip8.memory[(g_chip8.PC) + 1];
	
	// Decode & Execute
	dex_opcode(op);
}

/*
int main(int argc, char* argv[]){
	writeMessage(0, "This is an error");
	writeMessage(1, "This is a warning");
	writeMessage(2, "Piece of info");
	DEBUG_PRINT("debg failed lmao");
	return 0;
}
*/
