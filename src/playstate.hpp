
#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <irrKlang.h>
#include "gamestate.hpp"

using namespace irrklang;

class Tetromino;
class Board;

class PlayState : public GameState {
public:
    static const int NCOLORS = 7;
    static const int GAME_OFFSET = 20;       // space between board border and window border
    
    void init(GameEngine* game);
    void clean_up(GameEngine* game);

    void pause();
    void resume();
    void reset();

    void input(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static PlayState* Instance() { return &m_playstate; }

protected:
    PlayState() { }

private:
    static PlayState m_playstate;

    void release_tetromino();
    void draw_block(GameEngine* game, int x, int y, int k, SDL_Rect clips[]);
    void create_button(GameEngine* game, int x, int y, int width, int height, int color[]);
    float frame_rate(GameEngine* game, int *last_time, int *this_time);

    // Game objects
    Board *board;
    Tetromino *tetro;
    Tetromino *next_tetro;

    // Music
    ISoundEngine* music_engine;

    // Texture
    SDL_Texture* block_texture;

    // Fonts
    SDL_Color       white;
    TTF_Font*       font_pause;
    TTF_Font*       font_tetris;
    TTF_Font*       font_score_text;
    TTF_Font*       font_score;
    TTF_Font*       font_new_game;
    TTF_Font*       font_quit;
    TTF_Font*       font_game_over;

    SDL_Texture*    font_image_pause;         
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

    bool paused;
    bool game_over;         // true when player looses
    bool exit;              // true when player exits game
};

#endif
