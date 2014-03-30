
/*
 *  Tetris class declaration
 */

#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

class Tetromino;

class Tetris {
public:
    Tetris(int argc, char *argv[]);

    /* Screen resolution */
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    /* Window and renderer */
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    /* Game objects */
    Tetromino *tetro;           // current tetromino
    Tetromino *next_tetro;      // next tetromino

    /* Board information */
    static const int NUMROWS = 30;       // Number of rows in board
    static const int NUMCOLS = 15;       // numbers of columns in board

    /* Sounds */

    /* Controllers */

    /* Fonts */
    SDL_Color       font_color;
    SDL_Texture*    font_image_tetris;         
    SDL_Texture*    font_image_score_text;
    SDL_Texture*    font_image_score;
    SDL_Texture*    font_image_new_game;
    SDL_Texture*    font_image_quit;
    SDL_Texture*    font_image_game_over; 

    /* Scores */

    int xoffset;                    //  0 (no movement)
                                    // -1 (tetromino will move left)
                                    //  1 (tetromino will move right)

    bool rotate_left;       // true if rotation occured (always counterclockwise)
    bool shifted;           // true if tetromino was shifted left or right
    bool zoom_down;         // true if spacebar was pressed, falls down instantaneously
    bool speedup;           // true if 's' or 'down' was pressed, falls down rapidly

    bool launch_tetro;    // true when last tetrimino has landed

    bool newgamedown;    // true when player presses "New Game" button
    bool newgameup;    // true when player releases "New Game" button
    bool quitdown;    // true when player presses "Quit" button 
    bool quitup;    // true when player releases "Quit" button

    bool gameover;    // true when player looses
    bool done;    // true when player exits game

    bool delete_row;    // true when player fills a row
    int bonus;        // bonus given if bonus_counter = 4
    int bonus_counter;        // counts the number of consecutive row deletes
    int score;        // score (speed of fall is proportional to score)
    bool render_score;

    float gameoffset;    // space between board border and window border

    float acceleration;   // multiplied by score to provide falling speed

    float thisTime;     // time since SDL_Init() of the current game loop
    float lastTime;     // time since SDL_Init() of the previous game loop
    float deltaTime;     // thisTime - lastTime
    float time_till_drop;     // tetromino falls down 1 block every time_till_drop seconds 
    float time_counter;     // counts number of game loops to allow tetromino to fall down  

    int game_width;      // board width
    int game_height;      // board height

    // This is the board, where dropped tetrominos live
    int board[NUMROWS][NUMCOLS];    // it's [y][x] not [x][y]

    // Width and height of a block of the board
    float blockWidth;
    float blockHeight; 

    // Coordinates of the "New Game" button
    // Also used for "Quit" button
    float newgamex1;        // left
    float newgamex2;        // right
    float newgamey1;        // down
    float newgamey2;        // up


    static const int NCOLORS = 6;          // number of colors

    static const float colors[][4];

    /* Main functions */
    void execute();

    void input();
    void update();
    void render();

    void reset();
    void clean_up();

private:
    /* Helper functions */
    void draw_block(int x, int y, int k);
    void create_button(int x, int y, int width, int height, int k);

};

#endif
