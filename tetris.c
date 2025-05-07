#include <stdio.h>
#include <SDL2/SDL.h>

int main() {


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return 1;
    } 


    SDL_Window *window = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800, 
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black background
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000); //Show the window for 3 seconds

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}