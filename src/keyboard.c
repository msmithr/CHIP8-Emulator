#include "keyboard.h"
#include <SDL2/SDL.h>

// set the machine keypad state
void set_keys(chip8 *machine) {
    SDL_PumpEvents();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    for (int i = 0x0; i <= 0xf; i++) {
        machine->key[i] = keystates[sdlscancode_from_key(i)];
    }
}

// mapping key value -> sdl scancode
Uint8 sdlscancode_from_key(int key) {
    if (key == 0x0) return SDL_SCANCODE_X;
    if (key == 0x1) return SDL_SCANCODE_1;
    if (key == 0x2) return SDL_SCANCODE_2;
    if (key == 0x3) return SDL_SCANCODE_3;
    if (key == 0x4) return SDL_SCANCODE_Q;
    if (key == 0x5) return SDL_SCANCODE_W;
    if (key == 0x6) return SDL_SCANCODE_E;
    if (key == 0x7) return SDL_SCANCODE_A;
    if (key == 0x8) return SDL_SCANCODE_S;
    if (key == 0x9) return SDL_SCANCODE_D;
    if (key == 0xa) return SDL_SCANCODE_Z;
    if (key == 0xb) return SDL_SCANCODE_C;
    if (key == 0xc) return SDL_SCANCODE_4;
    if (key == 0xd) return SDL_SCANCODE_R;
    if (key == 0xe) return SDL_SCANCODE_F;
    if (key == 0xf) return SDL_SCANCODE_V;
    return -1;
}

// mapping sdl scancode -> key value 
int key_from_sdlscancode(Uint8 scancode) {
    if (scancode == SDL_SCANCODE_X) return 0x0;
    if (scancode == SDL_SCANCODE_1) return 0x1;
    if (scancode == SDL_SCANCODE_2) return 0x2;
    if (scancode == SDL_SCANCODE_3) return 0x3;
    if (scancode == SDL_SCANCODE_Q) return 0x4;
    if (scancode == SDL_SCANCODE_W) return 0x5;
    if (scancode == SDL_SCANCODE_E) return 0x6;
    if (scancode == SDL_SCANCODE_A) return 0x7;
    if (scancode == SDL_SCANCODE_S) return 0x8;
    if (scancode == SDL_SCANCODE_D) return 0x9;
    if (scancode == SDL_SCANCODE_Z) return 0xa;
    if (scancode == SDL_SCANCODE_C) return 0xb;
    if (scancode == SDL_SCANCODE_4) return 0xc;
    if (scancode == SDL_SCANCODE_R) return 0xd;
    if (scancode == SDL_SCANCODE_F) return 0xe;
    if (scancode == SDL_SCANCODE_V) return 0xf;
    return -1;
}

// check if the exit key is pressed
int check_for_exit() {
    SDL_PumpEvents();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    return keystates[SDL_SCANCODE_ESCAPE];   
}
