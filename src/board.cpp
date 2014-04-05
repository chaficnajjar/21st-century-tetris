
#include "board.hpp"

Board::Board() {
    // Initially no blocks on the board
    for (int i = 0; i < NUMROWS; i++)
        for (int j = 0; j < NUMCOLS; j++)
            color[i][j] = -1;               // Note: board[y][x] not board [x][y]
                                            // -1 stands for "No color"
    score = 0;
    render_score = true;

    bonus_counter = 0;

}

//void Board::shift_down() {

//}

void Board::add_score(int delta) {
    score += delta;
}

int Board::get_score() {
    return score;
}

void Board::delete_rows() {

    bool delete_row = false;

    // If row is full, shift board down 
    for (int i = NUMROWS-1; i >= 0; i--) {          // test every row

        if (!delete_row) {
            delete_row = true;                      // true until proven false
            for (int j = 0; j < NUMCOLS; j++) {     // check each block of the row 
                if (color[i][j] == -1) {            // if there exists at least one empty block 
                    delete_row = false;             // do not delete any row
                    break; 
                }
            }
        }

        // If delete_row is true, shift board down
        else 
            for (int j = 0; j < NUMCOLS; j++)
                color[i+1][j] = color[i][j];

    }

    // Row was deleted
    if (delete_row) {
        add_score(1);                    // increment score
        bonus_counter++;            // increment bonus counter 
        render_score = true;
        delete_row = false;             // row now deleted
    }

    else 
        bonus_counter = 0;          // tetromino has landed but no row was deleted
        
    // 4 rows deleted in a row (no pun intended)
    if (bonus_counter == 4) { 
           add_score(BONUS);      // you get 3 points // replace by constant BONUS
           bonus_counter = 0;
    }

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
