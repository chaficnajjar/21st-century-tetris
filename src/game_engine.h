// Copyright [2015] <Chafic Najjar>

#ifndef SRC_GAME_ENGINE_H_
#define SRC_GAME_ENGINE_H_

#include <SDL2/SDL.h>

#include <vector>

class GameState;

class GameEngine {
 public:
    GameEngine();

    void clean_up();

    void change_state(GameState* state);
    void push_state(GameState* state);
    void pop_state();

    void execute();
    void input();
    void update();
    void render();

    bool running() { return !exit; }
    void quit() { exit = true; }

    // Screen dimensions.
    int width;
    int height;

    // Window and renderer.
    SDL_Window* window;
    SDL_Renderer* renderer;

 private:
    // Stack of states.
    std::vector<GameState*> states;

    bool exit;
};

#endif  // SRC_GAME_ENGINE_H_
