//  Tetris game written in C++ using SDL 2.0
//  Author: Chafic Najjar <chafic.najjar@gmail.com>

#include "tetris.hpp"

int main(int argc, char *argv[]) {
    Tetris tetris(argc, argv);
    tetris.execute();
    return 0;
}
