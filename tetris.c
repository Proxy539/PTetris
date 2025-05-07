#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

int main() {


    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
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

    TTF_Font *font = TTF_OpenFont("AmazDooMLeft.ttf", 24);
    if (!font) {
        printf("Font loader error: %s\n", TTF_GetError());
        return 1;
    }
    

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Color tetrisNameColor = {73, 179, 243};
    SDL_Surface *tetrisNameSurface = TTF_RenderText_Solid(font, "TETRIS", tetrisNameColor);
    SDL_Texture *tetrisNameTexture = SDL_CreateTextureFromSurface(renderer, tetrisNameSurface);
    SDL_Rect tetrisNameDstrect = {250, 10, 300, 300};
    SDL_FreeSurface(tetrisNameSurface);

    SDL_Color proxyNameColor = {252, 64, 66};
    SDL_Surface *proxyNameSurface = TTF_RenderText_Solid(font, "By proxy", proxyNameColor);
    SDL_Texture *proxyNameTexture = SDL_CreateTextureFromSurface(renderer, proxyNameSurface);
    SDL_Rect proxyNameDstrect = {300, 250, 200, 100};
    SDL_FreeSurface(proxyNameSurface);

    SDL_Color menuColor = {255, 255, 255};

    SDL_Surface *startGameNameSurface = TTF_RenderText_Solid(font, "Start", menuColor);
    SDL_Texture *startGameNameTexture = SDL_CreateTextureFromSurface(renderer, startGameNameSurface);
    SDL_Rect startGameNameDstrect = {350, 350, 100, 100};
    SDL_FreeSurface(startGameNameSurface);

    SDL_Surface *exitNameSurface = TTF_RenderText_Solid(font, "Exit", menuColor);
    SDL_Texture *exitNameTexture = SDL_CreateTextureFromSurface(renderer, exitNameSurface);
    SDL_Rect exitNameDstrect = {350, 450, 100, 100};
    SDL_FreeSurface(exitNameSurface);

    SDL_Rect optionRect = {250, 360, 40, 40};

    bool downPressed = false;
    bool upPressed = false;


    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } 

            if (event.type == SDL_KEYDOWN) {
                downPressed = upPressed = false;
                if (event.key.keysym.sym == SDLK_DOWN) {
                    downPressed = true;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    upPressed = true;
                }
            }
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black background
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, tetrisNameTexture, NULL, &tetrisNameDstrect);
        SDL_RenderCopy(renderer, proxyNameTexture, NULL, &proxyNameDstrect);
        SDL_RenderCopy(renderer, startGameNameTexture, NULL, &startGameNameDstrect);
        SDL_RenderCopy(renderer, exitNameTexture, NULL, &exitNameDstrect);

        if (upPressed || downPressed) {
            if (downPressed) {
                SDL_Rect optionRect = {250, 460, 40, 40};
                SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
                SDL_RenderFillRect(renderer, &optionRect);
            } else if (upPressed) {
                SDL_Rect optionRect = {250, 360, 40, 40};
                SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
                SDL_RenderFillRect(renderer, &optionRect);
            }
        } else {
            SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
            SDL_RenderFillRect(renderer, &optionRect);
        }

        SDL_RenderPresent(renderer);

    }


    


    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}