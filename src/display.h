#include "chip8.h"

#define SCREEN_WIDTH_SCALE_FACTOR 20                                                                                                              
#define SCREEN_HEIGHT_SCALE_FACTOR 20
#define CHIP8_WIDTH 64 
#define CHIP8_HEIGHT 32

#define SCREEN_WIDTH CHIP8_WIDTH * SCREEN_WIDTH_SCALE_FACTOR
#define SCREEN_HEIGHT CHIP8_HEIGHT * SCREEN_HEIGHT_SCALE_FACTOR

// initialize the SDL2 display
int display_init();
// render the display based on the current display state of the machine
void render(chip8 *machine);
// make the beep sound
void beep();
