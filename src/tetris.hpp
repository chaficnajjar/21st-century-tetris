//  Tetris class declarations

#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Tetromino;
class Board;

class Tetris {
public:
    Tetris(int argc, char *argv[]);

    /* Screen resolution */
    static const int SCREEN_WIDTH = 500;
    static const int SCREEN_HEIGHT = 640;

    /* Window and renderer */
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    /* Game objects */
    Board *board;               // game board
    Tetromino *tetro;           // current tetromino
    Tetromino *next_tetro;      // next tetromino

    /* Sounds */

    /* Controllers */

    /* Fonts */
    SDL_Color       white;
    SDL_Texture*    font_image_tetris;         
    SDL_Texture*    font_image_score_text;
    SDL_Texture*    font_image_score;
    SDL_Texture*    font_image_new_game;
    SDL_Texture*    font_image_quit;
    SDL_Texture*    font_image_game_over; 

    /* Scores */

    int x_offset;                    //  0 (no movement)
                                    // -1 (tetromino will move left)
                                    //  1 (tetromino will move right)

    bool rotate_left;       // true if rotation occured (always counterclockwise)
    bool shifted;           // true if tetromino was shifted left or right
    bool free_fall;         // true if spacebar was pressed, falls down instantaneously
    bool speed_up;           // true if 's' or 'down' was pressed, falls down rapidly

    bool launch_tetro;    // true when last tetrimino has landed

    bool newgamedown;    // true when player presses "New Game" button
    bool newgameup;    // true when player releases "New Game" button
    bool quitdown;    // true when player presses "Quit" button 
    bool quitup;    // true when player releases "Quit" button

    bool game_over;    // true when player looses
    bool done;    // true when player exits game

    float gameoffset;    // space between board border and window border

    float acceleration;   // multiplied by score to provide falling speed

    int this_time;     // time since SDL_Init() of the current game loop
    int last_time;     // time since SDL_Init() of the previous game loop

    float time_till_drop;     // tetromino falls down 1 block every time_till_drop seconds 
    float time_counter;     // counts number of game loops to allow tetromino to fall down  

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
    void release_tetromino();
    void draw_block(int x, int y, int k);
    void create_button(int x, int y, int width, int height, int k);
    float frame_rate(int *last_time, int *this_time);

};

#endif
