
#include "board.hpp"

Board::Board() {
    // Initially no blocks on the board
    for (int i = 0; i < NUMROWS; i++)
        for (int j = 0; j < NUMCOLS; j++)
            color[i][j] = -1;               // Note: board[y][x] not board [x][y]
                                            // -1 stands for "No color"
}

