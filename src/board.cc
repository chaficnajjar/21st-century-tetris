// Copyright [2015] <Chafic Najjar>

#include "src/tetromino.h"
#include "src/board.h"

Board::Board() {
    score = 0;
    render_score = true;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
           // All blocks on the board are initially colorless.
            color[i][j] = -1;
}

bool Board::full_row(int row) {
    for (int col = 0; col < COLS; col++)  // Check each block of the row.
        if (color[row][col] == -1)  // If there exists at least one empty block.
            return false;
    return true;
}

void Board::shift_down(int i) {
    for (int row = i; row > 0; row--)
        for (int col = 0; col < COLS; col++)
            color[row][col] = color[row-1][col];
}

void Board::delete_full_rows() {
    int bonus_counter = 0;  // Counts the number of consecutive row deletes.

    // Test every row, starting from bottom row
    for (int row = ROWS-1; row >= 0; row--) {
        if (!full_row(row))
            continue;

        // To delete a row, shift the upper part of the board down.
        shift_down(row);
        row++;

        increase_score_by(1);
        bonus_counter++;
        render_score = true;
    }

    if (bonus_counter == 4)
       increase_score_by(BONUS);
}

bool Board::add(Tetromino *tetro) {
    for (int i = 0; i < tetro->SIZE; i++) {
        int x = tetro->get_block_x(i);
        int y = tetro->get_block_y(i);

        // Tetromino isn't added to the board if it touches the upper border.
        if (y <= 0)
            return false;
        else
            // Add tetromino: update color in corresponding board block.
            color[y][x] = tetro->type;
    }
    return true;
}
