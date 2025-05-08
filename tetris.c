#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define CELL_SIZE 50
#define ROWS 20
#define COLS 10

void draw_grid(SDL_Renderer *renderer) {
    //Set grid line color 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //Draw vertical lines 
    for (int i = 0; i <= COLS; i++) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, ROWS * CELL_SIZE);
    }

    //Draw horizontal lines
    for (int i = 0; i <= ROWS; i++) {
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, COLS * CELL_SIZE, i * CELL_SIZE);
    }
}

int main() {


    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return 1;
    } 


    SDL_Window *window = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        1000, 
        1000,
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

    bool menuRunning = true;
    bool gameRunning = false;
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

    bool startChosen = true;
    bool exitChosen = false;


    while (menuRunning) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                menuRunning = false;
            } 

            if (event.type == SDL_KEYDOWN) {                
                switch(event.key.keysym.sym) {
                    case SDLK_DOWN: exitChosen = true; startChosen = false; break;
                    case SDLK_UP: startChosen = true; exitChosen = false; break;
                    case SDLK_RETURN: {
                        if (exitChosen) {
                            menuRunning = false;
                        } else if (startChosen) {
                            printf("Start the game\n");
                            gameRunning = true;
                            menuRunning = false;
                        }
                    }; break;
                }   
            }
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black background
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, tetrisNameTexture, NULL, &tetrisNameDstrect);
        SDL_RenderCopy(renderer, proxyNameTexture, NULL, &proxyNameDstrect);
        SDL_RenderCopy(renderer, startGameNameTexture, NULL, &startGameNameDstrect);
        SDL_RenderCopy(renderer, exitNameTexture, NULL, &exitNameDstrect);

        if (exitChosen || startChosen) {
            if (startChosen) {
                SDL_Rect optionRect = {250, 360, 40, 40};
                SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
                SDL_RenderFillRect(renderer, &optionRect);
            } else if (exitChosen) {
                SDL_Rect optionRect = {250, 460, 40, 40};
                SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
                SDL_RenderFillRect(renderer, &optionRect);
            }
        } else {
            SDL_SetRenderDrawColor(renderer, 73, 179, 243, 255);
            SDL_RenderFillRect(renderer, &optionRect);
        }

        SDL_RenderPresent(renderer);

    }


    while (gameRunning) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } 

            if (event.type == SDL_KEYDOWN) {                
                switch(event.key.keysym.sym) {
                    
                }   
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); //Clear the current render buffer

        //Now draw your game scene (e.g. Tetris board, pieces, etc.)
        
        draw_grid(renderer);

        //Present the new frame

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