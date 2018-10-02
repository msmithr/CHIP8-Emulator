#include <SDL2/SDL.h>
#include <errno.h>
#include "display.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

// initialize the display
int display_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("%s\n", strerror(errno));
        return -1;
    }

    // set up the window and renderer
    window = SDL_CreateWindow(
        "Chip8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        printf("%s\n", strerror(errno));
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("%s\n", strerror(errno));
        return -1;
    }
     
    // set up audio
    SDL_LoadWAV("beep.wav", &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    return 0;
}

// update the display based on the current machine display state
void render(chip8 *machine) {
    SDL_RenderClear(renderer);

    for (int x = 0; x < CHIP8_WIDTH; x++) {
        for (int y = 0; y < CHIP8_HEIGHT; y++) {
            if (machine->disp[y*CHIP8_WIDTH + x]) {
                SDL_Rect rect;
                rect.x = x * SCREEN_WIDTH_SCALE_FACTOR;
                rect.y = y * SCREEN_WIDTH_SCALE_FACTOR;
                rect.w = SCREEN_WIDTH_SCALE_FACTOR;
                rect.h = SCREEN_HEIGHT_SCALE_FACTOR;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

// make the beep sound
void beep() {
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}
