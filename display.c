#include <SDL2/SDL.h>
#include <errno.h>
#include "display.h"
#include "config.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

int display_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("%s\n", strerror(errno));
        return -1;
    }

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
     
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        CHIP8_WIDTH,
        CHIP8_HEIGHT 
    );

    if (texture == NULL) {
        printf("%s\n", strerror(errno));
        return -1;
    }

    return 0;
}

void render(chip8 *machine) {
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
