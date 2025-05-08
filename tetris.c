#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#define CELL_SIZE 50
#define ROWS 20
#define COLS 10

int board[ROWS][COLS];
bool hasFigure = false;

void moveLeft() {

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j]) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            board[figureRowPos[i]][figureColPos[i] - 1] = figureColor;
        } 
}

void moveRight() {

    int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j]) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            board[figureRowPos[i]][figureColPos[i] + 1] = figureColor;
        } 
}

void generateSquare() {
    board[0][5] = 1;
    board[0][4] = 1;
    board[1][5] = 1;
    board[1][4] = 1;
}

void generateStick() {
    board[0][3] = 2;
    board[0][4] = 2;
    board[0][5] = 2;
    board[0][6] = 2;
}

void generateZ() {
    board[0][4] = 3;
    board[0][5] = 3;
    board[1][5] = 3;
    board[1][6] = 3;
}

void generateTriangle() {
    board[0][5] = 4;
    board[1][4] = 4;
    board[1][5] = 4;
    board[1][6] = 4;
}

void moveFigure() {

    srand(time(NULL));

    if (hasFigure) {

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j]) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;
                }
            }
        }

        if (count) {
            for (int i = 0; i < 4; i++) {
                if (figureRowPos[i] + 1 >= ROWS) {
                    hasFigure = false;
                    return;
                }

                board[figureRowPos[i] + 1][figureColPos[i]] = figureColor;
            }
        }
        
    } else {

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                board[i][j] = 0;
            }
        }

        int figureNumber = rand() % 4;
        printf("%d\n", figureNumber);

        switch(figureNumber) {
            case 0: generateSquare(); break;
            case 1: generateStick(); break;
            case 2: generateZ(); break;
            case 3: generateTriangle(); break; 
        }


        hasFigure = true;
    }
}

void draw_grid(SDL_Renderer *renderer) {
   SDL_Rect cell;

   for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            cell.x = col * CELL_SIZE;
            cell.y = row * CELL_SIZE;
            cell.w = CELL_SIZE;
            cell.h = CELL_SIZE;

            if (board[row][col]) {

                switch(board[row][col]) {
                    case 1: SDL_SetRenderDrawColor(renderer, 65,251,118, 255);break;
                    case 2: SDL_SetRenderDrawColor(renderer, 65,61,255, 255); break;
                    case 3: SDL_SetRenderDrawColor(renderer, 254,64,62, 255); break;
                    case 4: SDL_SetRenderDrawColor(renderer, 237,244,72, 255); break;
                }

                SDL_RenderFillRect(renderer, &cell);
            }

            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Border color
            SDL_RenderDrawRect(renderer, &cell);
        }
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


    if (gameRunning) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                board[i][j] = 0;
            }
        }
    }


    while (gameRunning) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            } 

            if (event.type == SDL_KEYDOWN) {                
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT: moveLeft(); break;
                    case SDLK_RIGHT: moveRight(); break;
                }   
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); //Clear the current render buffer

        //Now draw your game scene (e.g. Tetris board, pieces, etc.)
        
        draw_grid(renderer);
        moveFigure();

        SDL_Delay(200);

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