// Copyright [2015] <Chafic Najjar>

#include "src/game_engine.h"
#include "src/gamestate.h"

GameEngine::GameEngine() {
    // Initialize audio, CD-ROM, event handling, file I/O,
    // joystick handling, threading, timers and videos.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Screen dimensions.
    width = 500;
    height = 640;

    // Window and renderer.
    window = SDL_CreateWindow("Tetris Unleashed!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    exit = false;
}

void GameEngine::execute() {
    while (!exit) {
        input();
        update();
        render();
    }
    clean_up();
}

void GameEngine::clean_up() {
    // Clean up the current state.
    while (!states.empty()) {
        states.back()->clean_up(this);
        states.pop_back();
    }

    SDL_Quit();
}

void GameEngine::change_state(GameState* state) {
    // Clean up the current state.
    if (!states.empty()) {
        states.back()->clean_up(this);
        states.pop_back();
    }

    // Store and initialize the new state.
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::push_state(GameState* state) {
    // Pause current state.
    if ( !states.empty() )
        states.back()->pause();

    // Store and initialize the new state.
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::pop_state() {
    // Clean up the current state.
    if ( !states.empty() ) {
        states.back()->clean_up(this);
        states.pop_back();
    }

    // Resume previous state.
    if ( !states.empty() )
        states.back()->resume();
}


void GameEngine::input() {
    // Let the state handle events.
    states.back()->input(this);
}

void GameEngine::update() {
    // Let the state update the game.
    states.back()->update(this);
}

void GameEngine::render() {
    // Let the state draw the screen.
    states.back()->render(this);
}
