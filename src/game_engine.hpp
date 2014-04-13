
#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <vector>
#include <SDL2/SDL.h>

class GameState;

class GameEngine {
public:
    static const int SCREEN_WIDTH = 500;
    static const int SCREEN_HEIGHT = 640;

    GameEngine();

    void clean_up();

    void change_state(GameState* state);
    void push_state(GameState* state);
    void pop_state();

    void execute();
    void input();
    void update();
    void render();

    bool running() { return !exit; };
    void quit() { exit = true; };

    // Window and renderer
    SDL_Window* window;
    SDL_Renderer* renderer;

private:
    // Stack of states
    std::vector<GameState*> states;

    bool exit;
};

#endif
