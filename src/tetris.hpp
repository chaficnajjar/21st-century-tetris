//  Tetris class declarations

#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Tetromino;
class Board;

class Tetris {
public:
    static const int SCREEN_WIDTH = 500;
    static const int SCREEN_HEIGHT = 640;
    static const int NCOLORS = 6;
    static const int COLORS[][4];
    static const int GAME_OFFSET = 20;       // space between board border and window border

public: 
    Tetris(int argc, char *argv[]);
    ~Tetris();
    void execute();
    void input();
    void update();
    void render();
    void reset();

private:
    void release_tetromino();
    void draw_block(int x, int y, int k);
    void create_button(int x, int y, int width, int height, int k);
    float frame_rate(int *last_time, int *this_time);

private:
    // Window and renderer 
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    // Game objects
    Board *board;
    Tetromino *tetro;
    Tetromino *next_tetro;

    // Fonts
    SDL_Color       white;
    SDL_Texture*    font_image_tetris;         
    SDL_Texture*    font_image_score_text;
    SDL_Texture*    font_image_score;
    SDL_Texture*    font_image_new_game;
    SDL_Texture*    font_image_quit;
    SDL_Texture*    font_image_game_over; 

    // Frame rate
    float acceleration;     // multiplied by score to provide falling speed
    int this_time;          // time since SDL_Init() of the current game loop
    int last_time;          // time since SDL_Init() of the previous game loop
    float time_till_drop;   // tetromino falls down 1 block every time_till_drop seconds 
    float time_counter;     // counts number of game loops to allow tetromino to fall down  

    // Buttons status
    bool newgamedown;       // true when player presses "New Game" button
    bool newgameup;         // true when player releases "New Game" button
    bool quitdown;          // true when player presses "Quit" button 
    bool quitup;            // true when player releases "Quit" button

    // Buttons coordinates
    int newgamex1;
    int newgamex2;
    int newgamey1;
    int newgamey2;

    bool game_over;         // true when player looses
    bool exit;              // true when player exits game
};

#endif
