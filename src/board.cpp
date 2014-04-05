//  Board class definitions

#include "board.hpp"

Board::Board() {
    score = 0;
    render_score = true;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            color[i][j] = -1;   // All blocks on the board are initially colorless
}

void Board::increase_score_by(int delta) {
    score += delta;
}

int Board::get_score() {
    return score;
}

bool Board::full_row(int row) {
    for (int col = 0; col < COLS; col++)          // check each block of the row 
        if (color[row][col] == -1)              // if there exists at least one empty block 
            return false;
    return true;
}

void Board::shift_down(int i) {
    for (int row = i; row >= 0; row--)
        for (int col = 0; col < COLS; col++)
            color[row+1][col] = color[row][col];
}

void Board::delete_full_rows() {
    int bonus_counter = 0;                      // counts the number of consecutive row deletes

    for (int row = ROWS-1; row >= 0; row--) {         // test every row
        if (!full_row(row))
            continue;

        // To delete the row, shift the upper part of the board down
        shift_down(row);
        row++;

        increase_score_by(1);  
        bonus_counter++;
        render_score = true;
    }

    if (bonus_counter == 4)
       increase_score_by(BONUS);                // you get 3 points // replace by constant BONUS
}

bool Board::add(Tetromino *tetro) {
    for (int i = 0; i < tetro->SIZE; i++) { 

        int x = tetro->get_block_x(i);
        int y = tetro->get_block_y(i);

        // If any block touches the top border of the board, then it's game over
        if (y <= 0)
            return false; 
        else
            color[y][x] = tetro->color; // update color in corresponding block position
    }
    return true;
}
