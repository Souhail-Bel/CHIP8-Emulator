#include "isa.h"
#include "chip8.h"
#include "debug.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern Chip8 g_chip8;

static uint16_t currOPCODE;

// Instruction LUT
static InstructionHandler opcode_LUT[16];

// Decoding masks
#define F	((currOPCODE & 0xF000) >> 12)
#define X	((currOPCODE & 0x0F00) >> 8)
#define Y	((currOPCODE & 0x00F0) >> 4)
#define N	(currOPCODE  & 0x000F)
#define KK	(currOPCODE  & 0x00FF)
#define NNN	(currOPCODE  & 0x0FFF)

void init_opcode_tables(void){
	opcode_LUT[0x0] = &OP_SET_0;
	opcode_LUT[0x1] = &OP_1NNN;
	opcode_LUT[0x2] = &OP_2NNN;
	opcode_LUT[0x3] = &OP_3XKK;
	opcode_LUT[0x4] = &OP_4XKK;
	opcode_LUT[0x5] = &OP_5XY0;
	opcode_LUT[0x6] = &OP_6XKK;
	opcode_LUT[0x7] = &OP_7XKK;
	opcode_LUT[0x8] = &OP_SET_8;
	opcode_LUT[0x9] = &OP_9XY0;
	opcode_LUT[0xA] = &OP_ANNN;
	opcode_LUT[0xB] = &OP_BNNN;
	opcode_LUT[0xC] = &OP_CXKK;
	opcode_LUT[0xD] = &OP_DXYN;
	opcode_LUT[0xE] = &OP_SET_E;
	opcode_LUT[0xF] = &OP_SET_F;
}

void dex_opcode(uint16_t opcode){
	currOPCODE = opcode;
	DEBUG_PRINT("Executing 0x%04X...", opcode);
	opcode_LUT[F]();
}




#if 1 // STANDARD INSTRUCTIONS DEFINITIONS

void OP_UNDF(void){
	writeMessage(0, "Undefined opcode!");
	DEBUG_PRINT("OPCode: 0x%04X @PC: 0x%04X\n", currOPCODE, g_chip8.PC);
	exit(-1);
}


// SYS addr: Jump to a machine code routine at nnn (obsolete)
void OP_0NNN(void){
	return;
}

// CLS: clear display
void OP_00E0(void){
	memset(g_chip8.gfx, 0, DISP_S);
	g_chip8.flag_draw = 1;
	g_chip8.PC += 2;
}

// RET: return from a subroutine
void OP_00EE(void){
	if(g_chip8.stack_pointer == 0){
		writeMessage(0, "Stack underflow while returing from subroutine.");
		DEBUG_PRINT("OPCode: 0x%04X @PC: 0x%04X\n", currOPCODE, g_chip8.PC);
		exit(-1);
	}
	
	g_chip8.PC = g_chip8.stack[g_chip8.stack_pointer--];
	g_chip8.PC += 2;
}

// JP addr: jump to location nnn
void OP_1NNN(void){
	g_chip8.PC = NNN;
	DEBUG_PRINT("JP %03X", NNN);
}

// CALL addr: call subroutine at nnn
void OP_2NNN(void){
	if(g_chip8.stack_pointer >= STACK_NEST){
		writeMessage(0, "Stack overflow during subroutine call.");
		DEBUG_PRINT("OPCode: 0x%04X @PC: 0x%04X\n", currOPCODE, g_chip8.PC);
		exit(-1);
	}
	
	g_chip8.stack[++g_chip8.stack_pointer] = g_chip8.PC;
	g_chip8.PC	= NNN;
	DEBUG_PRINT("CALL %03X", NNN);
}

// SE Vx, byte: Skip next instruction if Vx = kk
void OP_3XKK(void){
	if(g_chip8.V[X] == KK) g_chip8.PC += 2;
	g_chip8.PC += 2;
	DEBUG_PRINT("SE V%X, %02X", X, KK);
}

// SNE Vx, byte: Skip next instruction if Vx != kk
void OP_4XKK(void){
	if(g_chip8.V[X] != KK) g_chip8.PC += 2;
	g_chip8.PC += 2;
	DEBUG_PRINT("SNE V%X, %02X", X, KK);
}

// SE Vx, Vy: Skip next instruction if Vx = Vy
void OP_5XY0(void){
	if(g_chip8.V[X] == g_chip8.V[Y]) g_chip8.PC += 2;
	g_chip8.PC += 2;
	DEBUG_PRINT("SE V%X, V%X", X, Y);
}

// LD Vx, byte: Set Vx = kk
void OP_6XKK(void){
	g_chip8.V[X] = KK;
	g_chip8.PC += 2;
	DEBUG_PRINT("LD V%X, %02X", X, KK);
}

// ADD Vx, byte: Set Vx += kk
void OP_7XKK(void){
	g_chip8.V[X] += KK;
	g_chip8.PC += 2;
	DEBUG_PRINT("ADD V%X, %02X", X, KK);
}

// LD Vx, Vy: Set Vx = Vy
void OP_8XY0(void){
	g_chip8.V[X] = g_chip8.V[Y];
	g_chip8.PC += 2;
	DEBUG_PRINT("LD V%X, V%X", X, Y);
}

// OR Vx, Vy: Set Vx |= Vy
void OP_8XY1(void){
	g_chip8.V[X] |= g_chip8.V[Y];
	g_chip8.PC += 2;
	DEBUG_PRINT("OR V%X, V%X", X, Y);
}

// AND Vx, Vy: Set Vx &= Vy
void OP_8XY2(void){
	g_chip8.V[X] &= g_chip8.V[Y];
	g_chip8.PC += 2;
	DEBUG_PRINT("AND V%X, V%X", X, Y);
}

// XOR Vx, Vy: Set Vx ^= Vy
void OP_8XY3(void){
	g_chip8.V[X] ^= g_chip8.V[Y];
	g_chip8.PC += 2;
	DEBUG_PRINT("XOR V%X, V%X", X, Y);
}

// ADD Vx, Vy: Set Vx += Vy, VF = carry
void OP_8XY4(void){
	uint16_t sum = g_chip8.V[X] + g_chip8.V[Y];
	g_chip8.V[0xF] = (sum > 255);
	g_chip8.V[X] = (uint8_t) sum;
	g_chip8.PC += 2;
	DEBUG_PRINT("ADD V%X, V%X", X, Y);
}

// SUB Vx, Vy: Set Vx -= Vy, VF = NOT borrow
void OP_8XY5(void){
	g_chip8.V[0xF] = g_chip8.V[X] >= g_chip8.V[Y];
	g_chip8.V[X] -= g_chip8.V[Y];
	g_chip8.PC += 2;
	DEBUG_PRINT("SUB V%X, V%X", X, Y);
}

// SHR Vx[, Vy]: Set Vx = Vx[Vy] SHR 1
void OP_8XY6(void){
	g_chip8.V[0xF] = g_chip8.V[X] & 0x1;
	g_chip8.V[X] >>= 1;
	g_chip8.PC += 2;
	DEBUG_PRINT("SHR V%X", X);
}

// SUBN Vx, Vy: Set Vx = Vy - Vx, VF = NOT borrow
void OP_8XY7(void){
	g_chip8.V[0xF] = g_chip8.V[X] >= g_chip8.V[Y];
	g_chip8.V[X] = g_chip8.V[Y] - g_chip8.V[X];
	g_chip8.PC += 2;
	DEBUG_PRINT("SUBN V%X, V%X", X, Y);
}

// SHL Vx[, Vy]: Set Vx = Vx[Vy] SHL 1
void OP_8XYE(void){
	g_chip8.V[0xF] = g_chip8.V[X] & 0x80;
	g_chip8.V[X] <<= 1;
	g_chip8.PC += 2;
	DEBUG_PRINT("SHL V%X", X);
}

// SNE Vx, Vy: Skip next instruction if Vx != Vy
void OP_9XY0(void){
	g_chip8.PC += 2*(1 + (g_chip8.V[X] != g_chip8.V[Y]));
	DEBUG_PRINT("SNE V%X, V%X", X, Y);
}

// LD I, addr: Set I = nnn
void OP_ANNN(void){
	g_chip8.I = NNN;
	g_chip8.PC += 2;
	DEBUG_PRINT("LD I, %03X", NNN);
}

// JP V0, addr: Jump to location V0 + nnn
void OP_BNNN(void){
	g_chip8.PC = g_chip8.V[0] + NNN;
	DEBUG_PRINT("JP V0, %03X", NNN);
}

// RND Vx, byte: Set Vx = random byte AND kk
void OP_CXKK(void){
	g_chip8.V[X] = (rand()%256) & KK;
	g_chip8.PC += 2;
	DEBUG_PRINT("RND V%X, %02X", X, KK);
}

// DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void OP_DXYN(void){
	byte cx = g_chip8.V[X];
	byte cy = g_chip8.V[Y];
	byte height = N;
	int idx = 0;
	
	// Set draw flag and reset collision flag
	g_chip8.flag_draw = 1;
	g_chip8.V[0xF] = 0;
	
	for(int line_y = 0; line_y < height; line_y++)
		for(int line_x = 0; line_x < 8; line_x++)
			if(g_chip8.memory[g_chip8.I + line_y] & (0x80 >> line_x)){
				idx = ((cx + line_x) % DISP_W)
					+ ((cy + line_y) % DISP_H) * DISP_W;
				
				if(g_chip8.gfx[idx]) g_chip8.V[0xF] = 1;
				g_chip8.gfx[idx] ^= 1;
			}

	g_chip8.PC += 2;
	DEBUG_PRINT("DRW V%X, V%X, %X", X, Y, N);
}

// SKP Vx: Skip next instruction if key with the value of Vx is pressed
void OP_EX9E(void){
	if(g_chip8.keys[g_chip8.V[X] & 0x0F]) g_chip8.PC += 2;
	g_chip8.PC += 2;
	DEBUG_PRINT("SKP V%X", X);
}

// SKNP Vx: Skip next instruction if key with the value of Vx is not pressed
void OP_EXA1(void){
	if(!g_chip8.keys[g_chip8.V[X] & 0x0F]) g_chip8.PC += 2;
	g_chip8.PC += 2;
	DEBUG_PRINT("SKNP V%X", X);
}

// LD Vx, DT: Set Vx = delay timer value
void OP_FX07(void){
	g_chip8.V[X] = g_chip8.timer_delay;
	g_chip8.PC += 2;
	DEBUG_PRINT("LD V%X, DT", X);
}

// LD Vx, K: Wait for a key press, store the value of the key in Vx
void OP_FX0A(void){
	for(int i = 0; i < KEY_NUMS; i++){
		if(g_chip8.keys[i]){
			g_chip8.V[X] = i;
			DEBUG_PRINT("LD V%X, K:%X", X, i);
			g_chip8.PC += 2;
			break;
		}
	}	
}

// LD DT, Vx: Set delay timer = Vx
void OP_FX15(void){
	g_chip8.timer_delay = g_chip8.V[X];
	g_chip8.PC += 2;
	DEBUG_PRINT("LD DT, V%X", X);
}

// LD ST, Vx: Set sound timer = Vx
void OP_FX18(void){
	g_chip8.timer_sound = g_chip8.V[X];
	g_chip8.PC += 2;
	DEBUG_PRINT("LD ST, V%X", X);
}

// ADD I, Vx: Set I += Vx
void OP_FX1E(void){
	g_chip8.I += g_chip8.V[X];
	g_chip8.PC += 2;
	DEBUG_PRINT("ADD I, V%X", X);
}

// LD F, Vx: Set I = location of sprite for digit Vx
void OP_FX29(void){
	g_chip8.I = g_chip8.V[X]*5 + FONT_OFFSET;
	g_chip8.PC += 2;
	DEBUG_PRINT("LD F, V%X", X);
}

// LD B, Vx: Store BCD representation of Vx in memory locations I, I+1 and I+2
void OP_FX33(void){
	g_chip8.memory[g_chip8.I  ] = g_chip8.V[X]/100;
	g_chip8.memory[g_chip8.I+1] = (g_chip8.V[X]/10)%10;
	g_chip8.memory[g_chip8.I+2] = g_chip8.V[X]%10;
	g_chip8.PC += 2;
	DEBUG_PRINT("LD B, V%X", X);
}

// LD {I}, Vx: Store registers V0 through Vx in memory starting at location I
void OP_FX55(void){
	for(int i = 0; i <= X; i++) g_chip8.memory[g_chip8.I + i] = g_chip8.V[i];
	g_chip8.PC += 2;
	DEBUG_PRINT("LD {I}, V%X", X);
}

// LD Vx, {I}: Read registers V0 through Vx from memory starting at location I
void OP_FX65(void){
	for(int i = 0; i <= X; i++) g_chip8.V[i] = g_chip8.memory[g_chip8.I + i];
	g_chip8.PC += 2;
	DEBUG_PRINT("LD V%X, {I}", X);
}
#endif


#if 1 // SETS (0, 8, E, F) DEFINITIONS

void OP_SET_0(void){
	switch(currOPCODE){
		case 0x00E0: OP_00E0(); break;
		case 0x00EE: OP_00EE(); break;
		default: OP_0NNN(); // though obsolete, i kinda still wanna add it
	}
}

void OP_SET_8(void){
	switch(N){
		case 0x0: OP_8XY0(); break;
		case 0x1: OP_8XY1(); break;
		case 0x2: OP_8XY2(); break;
		case 0x3: OP_8XY3(); break;
		case 0x4: OP_8XY4(); break;
		case 0x5: OP_8XY5(); break;
		case 0x6: OP_8XY6(); break;
		case 0x7: OP_8XY7(); break;
		case 0xE: OP_8XYE(); break;
		default: OP_UNDF();
	}
}

void OP_SET_E(void){
	switch(KK){
		case 0x9E: OP_EX9E(); break;
		case 0xA1: OP_EXA1(); break;
		default: OP_UNDF();
	}
}

void OP_SET_F(void){
	switch(KK){
		case 0x07: OP_FX07(); break;
		case 0x0A: OP_FX0A(); break;
		case 0x15: OP_FX15(); break;
		case 0x18: OP_FX18(); break;
		case 0x1E: OP_FX1E(); break;
		case 0x29: OP_FX29(); break;
		case 0x33: OP_FX33(); break;
		case 0x55: OP_FX55(); break;
		case 0x65: OP_FX65(); break;
		default: OP_UNDF();
	}
}

#endif
