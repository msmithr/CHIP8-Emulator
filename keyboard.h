#include <SDL2/SDL.h>
#include "chip8.h"

void set_keys(chip8 *machine);
Uint8 sdlscancode_from_key(int key);
int key_from_sdlscancode(Uint8 scancode);
