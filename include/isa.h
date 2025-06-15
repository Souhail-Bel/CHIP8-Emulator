#ifndef ISA_H_
#define ISA_H_

#include <stdint.h>

#include "chip8.h"

// Function pointer type
typedef void (*InstructionHandler)(void);

void init_opcode_tables(void);

void dex_opcode(uint16_t opcode);


// STANDARD INSTRUCTIONS
void OP_UNDF(void);

// SYS addr: Jump to a machine code routine at nnn (obsolete)
void OP_0NNN(void);

// CLS: clear display
void OP_00E0(void);

// RET: return from a subroutine
void OP_00EE(void);

// JP addr: jump to location nnn
void OP_1NNN(void);

// CALL addr: call subroutine at nnn
void OP_2NNN(void);

// SE Vx, byte: Skip next instruction if Vx = kk
void OP_3XKK(void);

// SNE Vx, byte: Skip next instruction if Vx != kk
void OP_4XKK(void);

// SE Vx, Vy: Skip next instruction if Vx = Vy
void OP_5XY0(void);

// LD Vx, byte: Set Vx = kk
void OP_6XKK(void);

// ADD Vx, byte: Set Vx += kk
void OP_7XKK(void);

// LD Vx, Vy: Set Vx = Vy
void OP_8XY0(void);

// OR Vx, Vy: Set Vx |= Vy
void OP_8XY1(void);

// AND Vx, Vy: Set Vx &= Vy
void OP_8XY2(void);

// XOR Vx, Vy: Set Vx ^= Vy
void OP_8XY3(void);

// ADD Vx, Vy: Set Vx += Vy, VF = carry
void OP_8XY4(void);

// SUB Vx, Vy: Set Vx -= Vy, VF = NOT borrow
void OP_8XY5(void);

// SHR Vx[, Vy]: Set Vx = Vx[Vy] SHR 1
void OP_8XY6(void);

// SUBN Vx, Vy: Set Vx = Vy - Vx, VF = NOT borrow
void OP_8XY7(void);

// SHL Vx[, Vy]: Set Vx = Vx[Vy] SHL 1
void OP_8XYE(void);

// SNE Vx, Vy: Skip next instruction if Vx != Vy
void OP_9XY0(void);

// LD I, addr: Set I = nnn
void OP_ANNN(void);

// JP V0, addr: Jump to location V0 + nnn
void OP_BNNN(void);

// RND Vx, byte: Set Vx = random byte AND kk
void OP_CXKK(void);

// DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void OP_DXYN(void);

// SKP Vx: Skip next instruction if key with the value of Vx is pressed
void OP_EX9E(void);

// SKNP Vx: Skip next instruction if key with the value of Vx is not pressed
void OP_EXA1(void);

// LD Vx, DT: Set Vx = delay timer value
void OP_FX07(void);

// LD Vx, K: Wait for a key press, store the value of the key in Vx
void OP_FX0A(void);

// LD DT, Vx: Set delay timer = Vx
void OP_FX15(void);

// LD ST, Vx: Set sound timer = Vx
void OP_FX18(void);

// ADD I, Vx: Set I += Vx
void OP_FX1E(void);

// LD F, Vx: Set I = location of sprite for digit Vx
void OP_FX29(void);

// LD B, Vx: Store BCD representation of Vx in memory locations I, I+1 and I+2
void OP_FX33(void);

// LD {I}, Vx: Store registers V0 through Vx in memory starting at location I
void OP_FX55(void);

// LD Vx, {I}: Read registers V0 through Vx from memory starting at location I
void OP_FX65(void);


// Sets based on first nibble
void OP_SET_0(void);
void OP_SET_8(void);
void OP_SET_E(void);
void OP_SET_F(void);
#endif