
#ifndef BOARD_HPP
#define BOARD_HPP

#include "tetromino.hpp"

class Board {
private:
    int score;
public:

    /* Board dimensions */
    static const int WIDTH  = 300;
    static const int HEIGHT = 600;

    /* Board information */
    static const int NUMROWS = 30;       // Number of rows in board
    static const int NUMCOLS = 15;       // numbers of columns in board

    /* Width and height of a block of the board */
    static const int BLOCK_WIDTH = WIDTH / NUMCOLS;
    static const int BLOCK_HEIGHT = HEIGHT / NUMROWS; 

    /* Score */
    bool render_score;
    static const int BONUS = 3;     // bonus given if bonus_counter = 4
    int bonus_counter;              // counts the number of consecutive row deletes


    /* Color */
    int color[NUMROWS][NUMCOLS];    // it's [y][x] not [x][y]

    /* Board status */
    bool delete_row;                // true when player fills a row

public:
    Board();

    void add_score(int delta);
    int get_score();
    void delete_rows();
    bool add(Tetromino *tetro);

};

#endif

