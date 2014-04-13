
//  Tetris game written in C++ using SDL 2.0
//  Author: Chafic Najjar <chafic.najjar@gmail.com>

#include "game_engine.hpp"
#include "introstate.hpp"

int main(int argc, char *argv[]) {
    GameEngine game;
    game.change_state(IntroState::Instance());
    game.execute();
    return 0;
}
