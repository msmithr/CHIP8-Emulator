#include <SDL2/SDL.h>
#include <errno.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

int display_init() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

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

int main() {
    display_init();
    return 0;
}
