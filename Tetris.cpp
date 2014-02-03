
/*
 * Author: Chafic Najjar
 * Tetris game in SDL and OpenGL
 * To-do: Restart music when new game
 * NOTE: The origin of the coordinate system is the upper-left point of the window
 */

// SDL graphics library header files
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// Music
#include "SDL_mixer.h"

// Header file containing class definitions
#include "Tetromino.h"
#include <iostream>
using namespace std;

// For random values
#include <ctime>

// For font
#include "FreeType.h"

int xoffset = 0;                //  0 (no movement)
                                // -1 (tetromino will move left)
                                //  1 (tetromino will move right)

bool rotleft        = false;    // true if rotation occured (always counterclockwise)
bool shifted        = false;    // true if tetromino was shifted left or right
bool zoomdown       = false;    // true if spacebar was pressed, falls down instantaneously
bool speedup        = false;    // true if 's' or 'down' was pressed, falls down rapidly

bool newtetro       = false;    // true when last tetrimino has landed

bool delete_row     = false;    // true when player fills a row

bool newgamedown    = false;    // true when player presses "New Game" button
bool newgameup      = false;    // true when player releases "New Game" button
bool quitdown       = false;    // true when player presses "Quit" button 
bool quitup         = false;    // true when player releases "Quit" button

bool gameover       = false;    // true when player looses
bool done           = false;    // true when player exits game

int bonus           = 3;        // bonus given if bonus_counter = 4
int bonus_counter   = 0;        // counts the number of consecutive row deletes
int score           = 0;        // score (speed of fall is proportional to score)

float gameoffset    = 20.0f;    // space between board border and window border

float acceleration  = 0.005f;   // multiplied by score to provide falling speed

float thisTime      = 0.0f;     // time since SDL_Init() of the current game loop
float lastTime      = 0.0f;     // time since SDL_Init() of the previous game loop
float deltaTime     = 0.0f;     // thisTime - lastTime
float time_till_drop= 0.3f;     // tetromino falls down 1 block every time_till_drop seconds 
float time_counter  = 0.0f;     // counts number of game loops to allow tetromino to fall down  

int ScreenWidth     = 500;      // screen resolution (width)
int ScreenHeight    = 640;      // screen resolution (height)
int GameWidth       = 300;      // board width
int GameHeight      = 600;      // board height
int ScreenBPP       = 32;       // bits per pixel

// Use OpenGL alongside SDL and request double buffer
/* Double buffer allows rendering to happen in the background and then rapidly swaps
   final drawing to screen (player won't see the rendering as it happens) */
int ScreenFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;

static const int NUMROWS = 30;  // Number of rows in board
static const int NUMCOLS = 15;  // numbers of columns in board

// This is the board, where dropped tetrominos live
int board[NUMROWS][NUMCOLS];    // it's [y][x] not [x][y]

// Width and height of a block of the board
GLfloat blockWidth = GLfloat(GameWidth)/GLfloat(NUMCOLS);
GLfloat blockHeight = GLfloat(GameHeight)/GLfloat(NUMROWS); 

// Coordinates of the "New Game" button
// Also used for "Quit" button
float newgamex1 = gameoffset+GameWidth+blockWidth;       // left
float newgamex2 = gameoffset+GameWidth+8*blockWidth;     // right
float newgamey1 = GameHeight-4*blockHeight;              // up
float newgamey2 = GameHeight-6*blockHeight;              // down


const int NCOLORS = 6;          // number of colors
const float colours[NCOLORS][3] = {

     { 1.0f, 0.0f, 0.0f },      // red
     { 0.0f, 1.0f, 0.0f },      // blue
     { 0.0f, 0.0f, 1.0f },      // turquoise
     { 1.0f, 1.0f, 0.0f },      // purple
     { 0.0f, 1.0f, 1.0f },      // brown
     { 1.0f, 0.0f, 1.0f }       // yellow

};

Tetromino *tetro        = new Tetromino ( rand()%7, rand()%NCOLORS );       // current tetromino
Tetromino *next_tetro   = new Tetromino ( rand()%7, rand()%NCOLORS );       // next tetromino

freetype::font_data game_font;  // holds font characteristics (eg: font size) 

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
    SDL_Surface* screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ScreenBPP, ScreenFlags);

    // NULL is where we put text which is the window title in menu bar when windows is minimized
    SDL_WM_SetCaption("Tetris", NULL); 

    // Sets up OpenGL double buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

    // Black background color
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); 

    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Orthographic projection
    glOrtho(0, ScreenWidth, ScreenHeight, 0, -1, 1);

    // Modelview transformation matrix
    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity();

    // Font and font size
    // http://www.urbanfonts.com/fonts/Bitwise.htm
    game_font.init("bitwise.ttf", 19);

    // Music
    int audio_rate = 26000;                 // soundtrack speed

    Uint16 audio_format = AUDIO_S16SYS;     
    int audio_channels = 1;
    int audio_buffers = 4096;
    Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
    Mix_Music *music = Mix_LoadMUS("Tetris.mp3");

    Mix_PlayMusic(music, -1);               // play music an infinite amount of time

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
void CreateButton(float x1, float x2, float y1, float y2) { // (left, right, up, down)

        glBegin( GL_QUADS );
            glVertex3f( x1, y1, 0 );                // left up
            glVertex3f( x1, y2, 0 );                // left down
            glVertex3f( x2, y2, 0 );                // right down
            glVertex3f( x2, y1, 0 );                // right up
        glEnd();

        glColor3f(1.0f,1.0f,1.0f);

}

// Render Tetromino block
void DrawBlock() {

    glBegin( GL_QUADS ); 
        glVertex3f( 0, 0, 0 );                      // up left
        glVertex3f( blockWidth, 0, 0 );             // up right
        glVertex3f( blockWidth, blockHeight, 0 );   // down right
        glVertex3f( 0, blockHeight, 0 );            // down left
    glEnd();

}

// Render result
void Render() {

    glClear(GL_COLOR_BUFFER_BIT);

    // White
    glColor3f(1.0f, 1.0f, 1.0f);

    // Write text
    freetype::print(game_font, (next_tetro->X-3)*blockWidth, GameHeight+gameoffset-blockHeight, "TETRIS\n\nSCORE: %d", score); 
    freetype::print(game_font, (next_tetro->X-3)*blockWidth, (NUMROWS-next_tetro->Y+4)*blockHeight, "Next Piece:");

    // Draw tetromino squares
    for (int i = 0; i < tetro->SIZE; i++) {

        // (x, y, z) coordinates of the translation vector
        glTranslatef( GLfloat(tetro->GetBlockX(i))*blockWidth + gameoffset,
                      GLfloat(tetro->GetBlockY(i))*blockHeight + gameoffset,
                      0 ); 

        // Preserve color
        int k = tetro->color;
        glColor3f(colours[k][0], colours[k][1], colours[k][2]);

        DrawBlock();

        // Resets matrix modified by translation
        glLoadIdentity(); 

    }

    if (!gameover)

        // Draw next tetromino
        for (int i = 0; i < next_tetro->SIZE; i++) {

            // (x, y, z) coordinates of the translation vector
            glTranslatef( GLfloat(next_tetro->GetBlockX(i))*blockWidth,
                          GLfloat(next_tetro->GetBlockY(i))*blockHeight,
                          0 );

            // Preserve color
            int k = next_tetro->color; 
            glColor3f(colours[k][0], colours[k][1], colours[k][2]);

            DrawBlock();

            // Resets matrix modified by translation
            glLoadIdentity();
        }

    // This is the board. Non-active tetrominos live here.
    for (int i = 0; i < NUMROWS; i++)
        for (int j = 0; j < NUMCOLS; j++)

            if (board[i][j]!=-1) {

                // (x, y, z) coordinates of the translation vector
                glTranslatef( GLfloat(j)*blockWidth + gameoffset,
                              GLfloat(i)*blockHeight + gameoffset,
                              0 );

                // Preserve color
                int k = board[i][j]; 
                glColor3f(colours[k][0], colours[k][1], colours[k][2]);

                DrawBlock();

                // Resets matrix modified by translation
                glLoadIdentity();
            }

    // Box surrounding board
    // Blue
    glColor3f(0.0f,0.0f,1.0f);

    // Draw left border
    glBegin(GL_LINES);
        glVertex3f(gameoffset, gameoffset, 0);
        glVertex3f(gameoffset, gameoffset+GameHeight, 0);
    glEnd();

    // Draw right border
    glBegin(GL_LINES);
        glVertex3f(gameoffset+GameWidth, gameoffset, 0);
        glVertex3f(gameoffset+GameWidth, gameoffset+GameHeight, 0);
    glEnd();

    // Draw upper border
    glBegin(GL_LINES);
        glVertex3f(gameoffset, gameoffset, 0);
        glVertex3f(gameoffset+GameWidth, gameoffset, 0);
    glEnd();

    // Draw bottom border
    glBegin(GL_LINES);
        glVertex3f(gameoffset, gameoffset+GameHeight, 0);
        glVertex3f(gameoffset+GameWidth, gameoffset+GameHeight, 0);
    glEnd();


    // If game is over, display "Game Over!"
    if (gameover) {
        glLoadIdentity();
        glColor3f(1.0f, 0.0f, 0.0f);
        freetype::print(game_font, newgamex1, ScreenHeight-newgamey1+4*blockWidth, "Game Over!");
    }

    // Blue
    glColor3f(0.0f,0.0f,1.0f);

    // Create "New Game" button
    CreateButton(newgamex1,newgamex2,newgamey1,newgamey2); //(left. right, up, down)
    glLoadIdentity();
    freetype::print(game_font, newgamex1+0.3*blockWidth, ScreenHeight-newgamey2-1.4*blockHeight, "New Game");

    // Red
    glColor3f(1.0f,0.0f,0.0f);

    // Create quit button
    CreateButton(newgamex1,newgamex2,newgamey1+4*blockHeight,newgamey2+4*blockHeight);
    glLoadIdentity();
    freetype::print(game_font, newgamex1+2.1*blockWidth, ScreenHeight-newgamey1-3.4*blockHeight, "Quit");

    // Swap buffers
    SDL_GL_SwapBuffers();

}

// Restarts game
void SetUpNewGame() { 

    for(int i = 0; i < NUMROWS; i++)
        for(int j = 0; j < NUMCOLS; j++)
            board[i][j] = -1; 

    score = 0;

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

        if(newgameup && newgamedown)
            SetUpNewGame();             

        if(quitup && quitdown)
            done = true;
    } while(!done);

    SDL_Quit();
    
}

int main(int argc, char *argv[]) {

    Initialize();
    srand(time(NULL));
    Execute();

    return 0;
}
