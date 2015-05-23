// Tetris game written in C++ using SDL 2.0.
// Copyright [2015] <Chafic Najjar>

#include "src/game_engine.h"
#include "src/introstate.h"

int main(int argc, char *argv[]) {
    GameEngine game;
    game.change_state(IntroState::Instance());
    game.execute();
}
