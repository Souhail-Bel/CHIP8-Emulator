#include "debug.h"
#include "chip8.h"
#include "hmi.h"
#include <SDL2/SDL.h>
#include <math.h>


#define CONTEXT_WIDTH 640
#define CONTEXT_HEIGHT 320

#define CONTEXT_SCALE (CONTEXT_WIDTH/DISP_W)

#define BG_COL 0, 0, 0, 255
#define FG_COL 255, 255, 255, 255

SDL_Window* w;
SDL_Renderer* r;
SDL_Event e;
unsigned short loop;
extern Chip8 g_chip8;


#define SAMPLE_RATE 44100
#define AMPLITUDE 8000

// Non-blocking audio state with mutex
typedef struct {
	double freq;
	double duration;
	double cursor;
	SDL_bool playing;
	SDL_mutex* mutex;
} AudioState;

AudioState audio_state;
SDL_AudioDeviceID audio_device;

void initHMI(void){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
		writeMessage(0, "SDL Initialisation Failed: %s", SDL_GetError());
		exit(-1);
	}
	//SDL_CreateWindowAndRenderer(CONTEXT_WIDTH, CONTEXT_HEIGHT, SDL_WINDOW_SHOWN, &w, &r);
	
	w = SDL_CreateWindow("CHIP-8 Emulator",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	CONTEXT_WIDTH, CONTEXT_HEIGHT,
	SDL_WINDOW_SHOWN);
	
	if(!w){
		writeMessage(0, "SDL Window Creation Failed: %s", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}
	
	DEBUG_PRINT("Created window.");
	
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	
	if(!r){
		writeMessage(0, "SDL Renderer Creation Failed: %s", SDL_GetError());
		SDL_DestroyWindow(w);
		SDL_Quit();
		exit(-1);
	}
	
	DEBUG_PRINT("Created renderer.");
	
	audio_state = (AudioState) {0, 0, 0, SDL_FALSE, SDL_CreateMutex()};
	
	if(!audio_state.mutex){
		writeMessage(0, "Failed to create Mutex: %s", SDL_GetError());
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		SDL_Quit();
		exit(-1);
	}
	
	DEBUG_PRINT("Created audio mutex.");
	
	SDL_AudioSpec wanted_spec = {
		.freq = SAMPLE_RATE,
		.format = AUDIO_S16SYS,
		.channels = 1,
		.samples = 4096,
		.callback = audioCallback,
		.userdata = &audio_state
	};
	SDL_AudioSpec obtained_spec;
	
	audio_device = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &obtained_spec, 0);
	if(!audio_device){
		writeMessage(0, "Audio device error: %s", SDL_GetError());
		SDL_DestroyMutex(audio_state.mutex);
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		SDL_Quit();
		exit(-1);
	}
	
	DEBUG_PRINT("Created audio device.");
	
	SDL_PauseAudioDevice(audio_device, 0);
	
	DEBUG_PRINT("Paused audio device.");
}

void inputHandler(void){
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
}

void updateVideo(void){
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

void closeSDL(void){
	SDL_CloseAudioDevice(audio_device);
	SDL_DestroyMutex(audio_state.mutex);
	
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	
	SDL_Quit();
}



// NOT SO SOUND logic below that almost killed a man

void audioCallback(void* userdata, uint8_t* stream, int len) {
	AudioState* state = (AudioState*) userdata;
	int16_t* buffer = (int16_t*) stream;
	
	SDL_LockMutex(state -> mutex);
	
	double t = 0, decay = 0;
	for(long unsigned int i = 0; i < len/sizeof(int16_t); i++){
		if(state -> playing && (state->duration > state -> cursor)){
			t = state->cursor/SAMPLE_RATE;
			
			decay = 1.0 - (state->cursor/state->duration);
			if(decay < 0) decay = 0;
			
			buffer[i] = (int16_t) (AMPLITUDE * decay * (sin(2 * M_PI * state->freq * t) > 0 ? 1 : -1));
			
			state->cursor++;
		} else {
			buffer[i] = 0;
			
			if(state -> playing && (state->duration <= state -> cursor)){
				state->playing = SDL_FALSE;
				state->cursor = 0;
			}
		}
	}
	
	SDL_UnlockMutex(state->mutex);
}

void beeper(AudioState* state, double freq, double duration_secs){
	if(!state) return;
	SDL_LockMutex(state->mutex);
	state->freq = freq;
	state->duration = duration_secs * SAMPLE_RATE;
	state->cursor = 0;
	state->playing = SDL_TRUE;
	SDL_UnlockMutex(state->mutex);
}

void bootBeep(void){
	beeper(&audio_state, 900, 0.8);
}

void beep(void){
	beeper(&audio_state, 1600, 0.08);
}
