#ifndef HMI_H_
#define HMI_H_
#include "debug.h"
#include "chip8.h"
#include <stdint.h>
#include <SDL2/SDL.h>

/* KEYBOARD LAYOUT,
mapped to the left half of AZERTY
1	2	3	4			1	2	3	C
A	Z	E	R	---\	4	5	6	D
Q	S	D	F	---/	7	8	9	E
W	X	C	V			A	0	B	F
*/

static const uint8_t KEYPAD[KEY_NUMS] = {
	SDLK_x,	// 0
	SDLK_1,	// 1
	SDLK_2,	// 2
	SDLK_3,	// 3
	SDLK_a, // 4
	SDLK_z, // 5
	SDLK_e, // 6
	SDLK_q, // 7
	SDLK_s, // 8
	SDLK_d, // 9
	SDLK_w, // A
	SDLK_c, // B
	SDLK_4, // C
	SDLK_r, // D
	SDLK_f, // E
	SDLK_v	// F
};

extern SDL_Window* w;
extern SDL_Renderer* r;

void initHMI(void); // Initialize Human-Machine interface
void audioCallback(void* userdata, uint8_t* stream, int len);
void inputHandler(void);
void updateVideo(void);
void bootBeep(void);
void beep(void); // this little maneuver's gonna cost us 51 years
void closeSDL(void);

#endif