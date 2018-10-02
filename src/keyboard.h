#include <SDL2/SDL.h>
#include "chip8.h"

// set the keypad state in the machine based on what keys are pressed
void set_keys(chip8 *machine);
// get the SDL scancode from the key value
Uint8 sdlscancode_from_key(int key);
// get the key value from the SDL scancode
int key_from_sdlscancode(Uint8 scancode);
// check if the exit key is pressed
int check_for_exit();
