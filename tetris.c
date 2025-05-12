#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#define CELL_SIZE 50
#define ROWS 20
#define COLS 10

int board[ROWS][COLS];
bool newFigure = false;

typedef enum { square, stick_horizontal, stick_vertical, z_left, z_up, triangle_up, triangle_right, triangle_buttom, triangle_left} Figure;
Figure currentFigure;

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

bool hasFullLine() {

    bool hasFullLine;

    for (int i = 0; i < 20; i++) {

        hasFullLine = true;

        for (int j = 0; j < 10; j++) {
            if (board[i][j] != -1) {
                hasFullLine = false;
            }
        }
    }

    return hasFullLine;
}

int getLineToClear() {

    int lineToClear;
    bool hasFullLine;

    for (int i = 19; i >= 0; i++) {

        hasFullLine = true;

        for (int j = 0; j < 10; j++) {
            if (board[i][j] != -1) {
                hasFullLine = false;
            }
        }


        if (hasFullLine) {
            return i;
        }
    }

    return lineToClear;
}

void checkFullLines() {

            while (hasFullLine()) {

                printf("===============================\n");
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 10; j++) {
                        printf("%3d", board[i][j]);
                    }
                    printf("\n");
                }

                int lineToClear = getLineToClear();

                printf("Line to clear %d\n", lineToClear);

                for (int i = 0; i < 10; i++) {
                    board[lineToClear][i] = 0;
                }

                for (int i = lineToClear; i >= 0; i--) {
                    for (int j = 0; j < 10; j++) {
                        if (board[i][j] == -1) {
                            int temp = board[i][j];
                            board[i][j] = board[i + 1][j];
                            board[i + 1][j] = temp;
                        }
                    }
                }

            }
}

void rotateFigure() {

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        bool hasCollision = false;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] > 0) {
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    figureColor = board[i][j];
                    board[i][j] = 0;
                    count++;
                }
            }
        } 


        if (currentFigure == square) {
            //do nothing. no point of rotate square
        } else if (currentFigure == stick_horizontal) {
            printf("move stick_horizontal\n");
            //move horizontal -> vertical
            int firstPointRow = figureRowPos[0];
            int firstPointCol = figureColPos[0];

            if (firstPointRow <= 16) {
                figureRowPos[1] = firstPointRow + 1;
                figureColPos[1] = firstPointCol;
                
                figureRowPos[2] = firstPointRow + 2;
                figureColPos[2] = firstPointCol;
    
                figureRowPos[3] = firstPointRow + 3;
                figureColPos[3] = firstPointCol;

                currentFigure = stick_vertical;
            } else {
                printf("Can't move stick_horizontal -> stick_vertical: not enough space\n");
            }

        } else if (currentFigure == stick_vertical) {
            //move vertical -> horizontal

            printf("move stick_vertical -> stick_horizontal\n");
            //move horizontal -> vertical
            int firstPointRow = figureRowPos[0];
            int firstPointCol = figureColPos[0];

            figureRowPos[1] = firstPointRow;
            figureColPos[1] = firstPointCol + 1;

            figureRowPos[2] = firstPointRow;
            figureColPos[2] = firstPointCol + 2;

            figureRowPos[3] = firstPointRow;
            figureColPos[3] = firstPointCol + 3;

            currentFigure = stick_horizontal;

        } else if (currentFigure == z_left) {
            //move z_left -> z_up

            figureRowPos[2] = figureRowPos[2] - 2;

            figureColPos[3] = figureColPos[3] - 2;

            currentFigure = z_up;

        } else if (currentFigure == z_up) {
            //move z_up -> z_left

            figureRowPos[0] = figureRowPos[0] + 2;
            figureColPos[3] = figureColPos[3] + 2;

            currentFigure = z_left;

        } else if (currentFigure == triangle_up) {
            //move triangle_up -> triangle_right

            figureRowPos[0] += 2;

            figureRowPos[1]--;
            figureColPos[1]++;
            
            currentFigure = triangle_right;

        } else if (currentFigure == triangle_right) {
            //move triangle_right -> triangle_botom

            figureRowPos[0]++;
            figureColPos[0]--;

            currentFigure = triangle_buttom;

        } else if (currentFigure == triangle_buttom) {
            //move triangle_bottom -> triangle_left

            figureRowPos[2]--;
            figureColPos[2]--;

            currentFigure = triangle_left;
        } else if (currentFigure == triangle_left) {
            //move triangle_left -> triangle_up
        
            figureRowPos[3]--;
            figureColPos[3]++;

            currentFigure = triangle_up;
        }

        for (int i = 0; i < 4; i++) {
            board[figureRowPos[i]][figureColPos[i]] = figureColor;
        }    
}

void generateNewFigure() {

    int figureNumber = rand() % 4;

    switch(figureNumber) {
        case 0: generateSquare(); currentFigure = square; break;
        case 1: generateStick(); currentFigure = stick_horizontal; break;
        case 2: generateZ(); currentFigure = z_left; break;
        case 3: generateTriangle(); currentFigure = triangle_up; break; 
    }
}

bool checkIfNewFigureNeeded() {

    bool newFigureNeeded = true;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (board[i][j] > 0) {
                newFigureNeeded = false;
            }

        }
    }


    return newFigureNeeded;

}

void checkColision() {

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        bool hasCollision = false;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] > 0) {
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                }
            }
        } 


        for (int i = 0; i < 4; i++) {

            if (board[figureRowPos[i] + 1][figureColPos[i]] == -1) {
                hasCollision = true;
                break;
            }

            if (figureRowPos[i] == 19) {
                hasCollision = true;
                break;
            }
        }

        if (hasCollision) {
            for (int i = 0; i < 4; i++) {
                board[figureRowPos[i]][figureColPos[i]] = -1;
            }
        }
}

void moveLeft() {

        bool hasCollision = false;

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] > 0) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;

                    if (j == 0 || j == 9) {
                        hasCollision = true;
                    }

                    if (i > 0 && i < 19) {
                        if (board[i][j - 1] == -1 || board[i][j + 1] == -1) {
                            hasCollision = true;
                        }
                    }
                }
            }
        }


        if (!hasCollision) {
            for (int i = 0; i < 4; i++) {
                board[figureRowPos[i]][figureColPos[i] - 1] = figureColor;
            } 
        } else {
            for (int i = 0; i < 4; i++) {
                board[figureRowPos[i]][figureColPos[i]] = figureColor;
            } 
        } 
}

void moveRight() {

        bool hasCollision = false;    

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] > 0) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;

                    if (j == 0 || j == 9) {
                        hasCollision = true;
                    }

                    if (i > 0 && i < 19) {
                        if (board[i][j - 1] == -1 || board[i][j + 1] == -1) {
                            hasCollision = true;
                        }
                    }

                    
                }
            }
        }

        if (!hasCollision) {
            for (int i = 0; i < 4; i++) {
                board[figureRowPos[i]][figureColPos[i] + 1] = figureColor;
            } 
        } else {
            for (int i = 0; i < 4; i++) {
                board[figureRowPos[i]][figureColPos[i]] = figureColor;
            } 
        }
}

void fallDown() {

        int figureRowPos[4] = {0, 0, 0, 0};
        int figureColPos[4] = {0, 0, 0, 0};
        int count = 0;
        int figureColor;
        
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (board[i][j] > 0) {
                    figureColor = board[i][j];
                    figureRowPos[count] = i;
                    figureColPos[count] = j;
                    count++;
                    board[i][j] = 0;
                }
            }
        }



        for (int i = 0; i < 4; i++) {
            while (board[figureRowPos[i] + 1][figureColPos[i]] != -1 && figureRowPos[i] < 19) {
                figureRowPos[i]++;
            }

            board[figureRowPos[i]][figureColPos[i]] = -1;
        }
        


        

}

void moveDown() {

    int figureRowPos[4] = {0, 0, 0, 0};
    int figureColPos[4] = {0, 0, 0, 0};
    int count = 0;
    int figureColor;
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] > 0) {
                figureColor = board[i][j];
                figureRowPos[count] = i;
                figureColPos[count] = j;
                count++;
                board[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        board[figureRowPos[i] + 1][figureColPos[i]] = figureColor;
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
                    case -1: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
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

    srand(time(NULL));


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
                    case SDLK_UP: rotateFigure(); break;
                    case SDLK_DOWN: fallDown(); break;
                }   
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        if (checkIfNewFigureNeeded()) {
            generateNewFigure();
        }

        draw_grid(renderer);
        moveDown();
        checkColision();
        checkFullLines();

        SDL_Delay(400);


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