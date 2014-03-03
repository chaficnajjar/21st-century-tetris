
/*
 * Author: Chafic Najjar
 * Tetris game written in C++ and uses SDL
 * NOTE: The origin of the coordinate system is the upper-left point of the window
 */

#include "tetromino.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>   // music
#include <iostream>
using namespace std;

#include <ctime>                // random values

int xoffset = 0;                //  0 (no movement)
                                // -1 (tetromino will move left)
                                //  1 (tetromino will move right)

bool rotleft        = false;    // true if rotation occured (always counterclockwise)
bool shifted        = false;    // true if tetromino was shifted left or right
bool zoomdown       = false;    // true if spacebar was pressed, falls down instantaneously
bool speedup        = false;    // true if 's' or 'down' was pressed, falls down rapidly

bool newtetro       = false;    // true when last tetrimino has landed

bool newgamedown    = false;    // true when player presses "New Game" button
bool newgameup      = false;    // true when player releases "New Game" button
bool quitdown       = false;    // true when player presses "Quit" button 
bool quitup         = false;    // true when player releases "Quit" button

bool gameover       = false;    // true when player looses
bool done           = false;    // true when player exits game

bool delete_row     = false;    // true when player fills a row
int bonus           = 3;        // bonus given if bonus_counter = 4
int bonus_counter   = 0;        // counts the number of consecutive row deletes
int score           = 0;        // score (speed of fall is proportional to score)
bool render_score   = false;

float gameoffset    = 20.0f;    // space between board border and window border

float acceleration  = 0.005f;   // multiplied by score to provide falling speed

float thisTime      = 0.0f;     // time since SDL_Init() of the current game loop
float lastTime      = 0.0f;     // time since SDL_Init() of the previous game loop
float deltaTime     = 0.0f;     // thisTime - lastTime
float time_till_drop= 0.3f;     // tetromino falls down 1 block every time_till_drop seconds 
float time_counter  = 0.0f;     // counts number of game loops to allow tetromino to fall down  

int GameWidth       = 300;      // board width
int GameHeight      = 600;      // board height

const int SCREEN_WIDTH  = 500;
const int SCREEN_HEIGHT = 640;

const int NUMROWS   = 30;       // Number of rows in board
const int NUMCOLS   = 15;       // numbers of columns in board

// This is the board, where dropped tetrominos live
int board[NUMROWS][NUMCOLS];    // it's [y][x] not [x][y]

// Width and height of a block of the board
float blockWidth = float(GameWidth)/float(NUMCOLS);
float blockHeight = float(GameHeight)/float(NUMROWS); 

// Coordinates of the "New Game" button
// Also used for "Quit" button
float newgamex1 = gameoffset+GameWidth+blockWidth;       // left
float newgamex2 = gameoffset+GameWidth+8*blockWidth;     // right
float newgamey1 = GameHeight-4*blockHeight;              // down
float newgamey2 = GameHeight-6*blockHeight;              // up


const int NCOLORS = 6;          // number of colors
const float colors[NCOLORS][4] = {

     { 255, 0  , 0  , 255 },      // red
     { 0  , 255, 0  , 255 },      // blue
     { 0  , 0  , 255, 255 },      // turquoise
     { 255, 255, 0  , 255 },      // purple
     { 0  , 255, 255, 255 },      // brown
     { 255, 0  , 255, 255 }       // yellow

};

Tetromino *tetro        = new Tetromino ( rand()%7, rand()%NCOLORS );       // current tetromino
Tetromino *next_tetro   = new Tetromino ( rand()%7, rand()%NCOLORS );       // next tetromino

SDL_Window*     window;
SDL_Renderer*   renderer;
SDL_Color       font_color = { 255, 255, 255 };
SDL_Texture*    font_image_tetris;         
SDL_Texture*    font_image_score_text;
SDL_Texture*    font_image_score;
SDL_Texture*    font_image_new_game;
SDL_Texture*    font_image_quit;
SDL_Texture*    font_image_game_over; 
 
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr) {
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }

    else
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    renderTexture(tex, ren, dst, clip);
}

SDL_Texture* renderText(const string &message, const string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);

    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

void Initialize() {

    // At the start of the game:
    // x position of (0, 0) block of tetro is int(15/2) = 7 which is the exact horizontal middle of board
    // y position of (0, 0) block of tetro is 0 which is the top of the board
    tetro->SetPosition(int(NUMCOLS/2), 0);

    // Position next_tetro at the upper right of the window, outside of the board
    next_tetro->SetPosition(NUMCOLS+5,int(0.3*NUMROWS)); 

    // No blocks on the board
    for (int i = 0; i < NUMROWS; i++)
        for (int j = 0; j < NUMCOLS; j++)
            board[i][j] = -1;               // Note: board[y][x] not board [x][y]
                                            // -1 stands for "No color"

    // Initialize audio, CD-ROM, event handling, file I/O, joystick handling, threading, timers and videos
    SDL_Init(SDL_INIT_EVERYTHING);

    // Create window
    window = SDL_CreateWindow( "Tetris Unleashed!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN );

    // Create renderer
    renderer = SDL_CreateRenderer( window,
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    // Font
    TTF_Init();

    // Write text
    font_image_tetris = renderText("Tetris Unleashed!", "resources/fonts/bitwise.ttf", font_color, 16, renderer);
    font_image_score_text = renderText("Score: ", "resources/fonts/bitwise.ttf", font_color, 20, renderer);
    font_image_score = renderText(to_string(score), "resources/fonts/bitwise.ttf", font_color, 20, renderer);
    font_image_new_game = renderText("New game", "resources/fonts/bitwise.ttf", font_color, 20, renderer);
    font_image_quit = renderText("Quit", "resources/fonts/bitwise.ttf", font_color, 20, renderer);
    font_image_game_over = renderText("Game over!", "resources/fonts/bitwise.ttf", font_color, 16, renderer);

    // Music
    //int audio_rate = 26000;                 // soundtrack speed

    //Uint16 audio_format = AUDIO_S16SYS;     
    //int audio_channels = 1;
    //int audio_buffers = 4096;
    //Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
    //Mix_Music *music = Mix_LoadMUS("Tetris.mp3");

    //Mix_PlayMusic(music, -1);               // play music an infinite amount of time

}


// Get player input
void GetInput() {

    // Queuing events
    SDL_Event event;
    while (SDL_PollEvent(&event)) { 

        // Clicking 'x' or pressing F4
        if (event.type == SDL_QUIT) 
            done = true;

        // Key is pressed
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    done = true; 
                    break;
                case SDLK_a: case SDLK_LEFT:
                    xoffset = -1;
                    shifted = true;
                    break;
                case SDLK_d: case SDLK_RIGHT:
                    xoffset = 1;
                    shifted = true;
                    break;
                case SDLK_w: case SDLK_UP: 
                    if(tetro->type != 4)        // type 4 is the square
                        rotleft = true;
                    break;
                case SDLK_s: case SDLK_DOWN:
                    speedup = true;
                    break;
                case SDLK_SPACE:
                    zoomdown = true; 
                    break;
                default: 
                    break;
            }

        }

        // Key is released
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_s: case SDLK_DOWN:
                    speedup = false;
                    break;
                default:
                    break;
            } 
        }

        // Mouse moves
        if (event.type == SDL_MOUSEMOTION) {

            // Outside of the board
            if (event.motion.x > GameWidth + gameoffset) 
                SDL_ShowCursor(1);          // show cursor
            
            // Inside the board
            else
                SDL_ShowCursor(0);          // don't show cursor
        }

        // Mouse button clicked
        if (event.type == SDL_MOUSEBUTTONDOWN)
            switch (event.button.button) {

                // Left mouse button clicked
                case SDL_BUTTON_LEFT:
                    if (event.button.x > newgamex1 && event.button.x < newgamex2) {

                        // And mouse cursor is on "New Game" button 
                        if (event.button.y > newgamey2 && event.button.y < newgamey1)
                            newgamedown = true;

                        // And mouse cursor is on "Quit" button
                        else if (event.button.y > newgamey2+4*blockHeight && event.button.y < newgamey1+4*blockHeight)
                            quitdown = true;
                    }
                    break;
                default:
                    break;
            }

        // Mouse button released 
        if (event.type == SDL_MOUSEBUTTONUP)
            switch (event.button.button) {

                // Left mouse button released
                case SDL_BUTTON_LEFT:
                    if (event.button.x > newgamex1 && event.button.x < newgamex2) {

                        // And mouse cursor is on "New Game" button
                        if (event.button.y > newgamey2 && event.button.y < newgamey1)
                            newgameup = true;

                        // And mouse cursor is on "Quit" button
                        else if (event.button.y > newgamey2+4*blockHeight && event.button.y < newgamey1+4*blockHeight)
                            quitup = true;
                    }
                    break;
                default:
                    break;
            }
    }
}

// Update game values
void Update() { 

    // Get number of milliseconds since SDL_Init() of the previous frame
    lastTime = thisTime; 

    // Get number of milliseconds since SDL_Init()
    thisTime = SDL_GetTicks(); 

    // Variation of time between each game loop
    deltaTime = (thisTime - lastTime) / 1000.0;         // dividing by 1000 to convert in seconds

    // No gameover (tetromino hasn't crossed the top border)
    if (!gameover) { 

        // Tetromino has landed
        if (newtetro) { 
            int x, y;

            // Check if tetromino has trespassed over the top border of the board
            for (int i = 0; i < tetro->SIZE; i++) { 

                x = tetro->GetBlockX(i);
                y = tetro->GetBlockY(i);

                // If any block touches the top border of the board, then it's game over
                if (y <= 0) { 
                    gameover = true;
                    return; 
                }

                board[y][x] = tetro->color; // update color in corresponding block position

            }

            // New tetromino
            Tetromino *new_tetro;
            new_tetro = new Tetromino(rand()%7, rand()%NCOLORS);        // assign random type and color to new tetromino
            new_tetro->SetPosition(next_tetro->X, next_tetro->Y);       // new tetrominoe is positioned under "Next Piece"

            delete [] tetro;                        // delete allocated memory
            tetro = next_tetro;                     // update falling tetromino
            tetro->SetPosition(int(NUMCOLS/2), 0);  // position the newly falling tetromino
            next_tetro = new_tetro;                 // update next_tetro to point to tetromino under "Next Piece"
            newtetro = false;                       // old falling tetromino has landed and new tetromino has already been generated
            zoomdown = false;                       // previous tetromino has landed so zoomdown is necessarly false
        }

        // No zoomdown
        if (!zoomdown) {

            // Player has rotated tetromino
            if (rotleft)
                tetro->RotateLeft();

            // Update tetromino position on the x-axis
            tetro->X += xoffset; 

            time_counter += deltaTime;

            // time_till_drop = 0.3;
            // deltaTime ~ 0.017 seconds
            // Tetromino falls one block for every 0.3/0.017 = 17 game loops
            if (time_counter >= time_till_drop) { 
                tetro->Y++; 
                time_counter = 0.0f;
            }

            // No speedup
            if (!speedup) 
                // Drop speed proportional to score
                time_till_drop = 0.3f - score*acceleration; 

            // Speedup ('s' or 'down') was pressed
            else
                time_till_drop = 0.02f; // 2x slower than zoomdown
        }

        // Zoomdown
        else 
            // The speed of fall is not decreased, the tetromino falls at speed proportional to the game loop
            tetro->Y++; 

        // Check if tetromino is in an acceptable place. If not, undo previous move(s)
        int x, y;
        
        // Collision detection
        for (int i = 0; i < tetro->SIZE; i++) {

            // Coordinates of falling tetromino
            x = tetro->GetBlockX(i);
            y = tetro->GetBlockY(i); 

            // Block crosses wall after rotation or translation 
            if (x < 0 || x >= NUMCOLS) {

                // If it exceeds the wall because of rotation
                if (rotleft) 
                    tetro->RotateRight();       // neutralize the left rotation 

                // Else it exceeds the wall because of translation
                else 
                    tetro->X -= xoffset;        // neutralize shift
            }

            // Block bypasses top border of the board 
            else if (y >= NUMROWS) {
                newtetro = true;                // cast a new tetrimino
                tetro->SetBlockY(i, NUMROWS-1); // change the value of Y so that block(s) of the (old) tetromino is/are above the blue line
            }

            // Block is on the board
            else if (y >= 0) 

                // Block touched another block
                if (board[y][x] != -1) { 

                    // Tetromino rotates and collides with a block
                    if (rotleft) { 
                        tetro->RotateRight();           // neutralize
                        rotleft = false;                // done rotating
                    } 
                    
                    // Tetromino is shifted into another block
                    else if (shifted) { 
                        tetro->X -= xoffset;            // neutralize
                        shifted = false;                // done shifting
                    } 
                    
                    // Block is neither rotated or shifted
                    else { 
                        tetro->Y--;                     // neutralize: tetromino goes up
                        newtetro = true;                // done landing
                    }

                    // Next block to test
                    x = tetro->GetBlockX(i);
                    y = tetro->GetBlockY(i);
                }
        }

        // If row is full, shift board down 
        for (int i = NUMROWS-1; i >= 0; i--) {          // test every row

            if (!delete_row) {
                delete_row = true;                      // true until proven false
                for (int j = 0; j < NUMCOLS; j++) {     // check each block of the row 
                    if (board[i][j] == -1) {            // if there exists at least one empty block 
                        delete_row = false;             // do not delete any row
                        break; 
                    }
                }
            }

            // If delete_row is true, shift board down
            else 
                for (int j = 0; j < NUMCOLS; j++)
                    board[i+1][j] = board[i][j];

        }

        // Row was deleted
        if (delete_row) {
            score++;                    // increment score
            bonus_counter++;            // increment bonus counter 
            render_score = true;
        }

        else 
            bonus_counter = 0;          // tetromino has landed but no row was deleted
        
        delete_row = false;             // row now deleted

    }

    // Done rotating and shifting
    rotleft = false; 
    shifted = false;
    xoffset = 0;

    // 4 rows deleted in a row (no pun intended)
    if (bonus_counter == 4) { 
           score += bonus;      // you get 3 points
           bonus_counter = 0;
    }

}

// Create "New Game" and "Quit" buttons
void CreateButton(int x, int y, int width, int height, int k) {

        SDL_Rect rect = { x, y, width, height };
        SDL_SetRenderDrawColor(renderer, colors[k][0], colors[k][1], colors[k][2], colors[k][3]);
        SDL_RenderFillRect(renderer, &rect);

}

// Render Tetromino block
void DrawBlock(int x, int y, int k) {

    SDL_Rect block = {x, y, int(blockWidth), int(blockHeight)};
    SDL_SetRenderDrawColor(renderer, colors[k][0], colors[k][1], colors[k][2], colors[k][3]);
    SDL_RenderFillRect(renderer, &block);

}

// Render result
void Render() {

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);


    int x = (next_tetro->X-3)*blockWidth; 
    int y = gameoffset; 

    renderTexture(font_image_tetris, renderer, x, y);

    // Render score text
    renderTexture(font_image_score_text, renderer, x, y + blockWidth);

    // Render score
    if (render_score) {
        font_image_score = renderText(to_string(score), "bitwise.ttf", font_color, 20, renderer);
        render_score = false;
    }
    renderTexture(font_image_score, renderer, x + 60, y + blockWidth);
    
    int tetro_x, tetro_y;

    // Draw tetromino squares
    for (int i = 0; i < tetro->SIZE; i++) {

        // Get new coordinates
        tetro_x = float(tetro->GetBlockX(i))*blockWidth + gameoffset;
        tetro_y = float(tetro->GetBlockY(i))*blockHeight + gameoffset;

        DrawBlock(tetro_x, tetro_y, tetro->color);

    }

    if (!gameover)

        // Draw next tetromino
        for (int i = 0; i < next_tetro->SIZE; i++) {


            // Get new coordinates
            tetro_x = float(next_tetro->GetBlockX(i))*blockWidth;
            tetro_y = float(next_tetro->GetBlockY(i))*blockHeight;

            DrawBlock(tetro_x, tetro_y, next_tetro->color);

        }

    // This is the board. Non-active tetrominos live here.
    for (int i = 0; i < NUMROWS; i++)
        for (int j = 0; j < NUMCOLS; j++)

            if (board[i][j]!=-1) {


                // Get new coordinates
                tetro_x = float(j)*blockWidth + gameoffset;
                tetro_y = float(i)*blockHeight + gameoffset;

                DrawBlock(tetro_x, tetro_y, board[i][j]);

            }

    
    // Box surrounding board

    // Set color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Draw left border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset, gameoffset, gameoffset+GameHeight);

    // Draw right border
    SDL_RenderDrawLine(renderer, gameoffset+GameWidth, gameoffset, gameoffset+GameWidth, gameoffset+GameHeight);

    // Draw upper border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset, gameoffset+GameWidth, gameoffset);

    // Draw bottom border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset+GameHeight, gameoffset+GameWidth, gameoffset+GameHeight);

    // If game is over, display "Game Over!"
    if (gameover)
        renderTexture(font_image_game_over, renderer, newgamex1, SCREEN_HEIGHT-newgamey1+4*blockWidth);

    // Create "New Game" button
    CreateButton(newgamex1, newgamey2, 7*blockWidth, 2*blockHeight, 2);

    // Render "New Game" font
    renderTexture(font_image_new_game, renderer, newgamex1+10, newgamey2+10);

    // Create "Quit" button
    CreateButton(newgamex1, newgamey2+4*blockHeight, 7*blockWidth, 2*blockHeight, 0);

    // Render "Quit" font
    renderTexture(font_image_quit, renderer, newgamex1+10, newgamey2+4*blockHeight+10);

    // Swap buffers
    SDL_RenderPresent(renderer);

}

// Restarts game
void SetUpNewGame() { 

    for(int i = 0; i < NUMROWS; i++)
        for(int j = 0; j < NUMCOLS; j++)
            board[i][j] = -1; 

    score = 0;
    render_score = true;

    // release allocated memory
    delete [] tetro;
    delete [] next_tetro;

    tetro = new Tetromino(rand()%7, rand()%NCOLORS ); 
    next_tetro = new Tetromino(rand()%7, rand()%NCOLORS );
    tetro->SetPosition(int(NUMCOLS/2),0);
    next_tetro->SetPosition(NUMCOLS+5,int(0.3*NUMROWS));

    rotleft         = false;
    shifted         = false;
    zoomdown        = false;
    
    newtetro        = false;
    gameover        = false;
    newgameup       = false;
    newgamedown     = false;

    bonus_counter   = 0;

}

// Execute game loop
void Execute() {

    do {

        GetInput();
        Update();
        Render();
        //SDL_Delay(1);

        if(newgameup && newgamedown)
            SetUpNewGame();             

        if(quitup && quitdown)
            done = true;

    } while(!done);

    SDL_DestroyTexture(font_image_tetris);
    SDL_DestroyTexture(font_image_score_text);
    SDL_DestroyTexture(font_image_score);
    SDL_DestroyTexture(font_image_new_game);
    SDL_DestroyTexture(font_image_quit);
    SDL_DestroyTexture(font_image_game_over);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

int main(int argc, char *argv[]) {

    Initialize();
    srand(time(NULL));
    Execute();

    return 0;
}
