
#ifndef BOARD_HPP
#define BOARD_HPP


class Board {
public:
    /* Board dimensions */
    static const int WIDTH  = 300;
    static const int HEIGHT = 600;

    /* Board information */
    static const int NUMROWS = 30;       // Number of rows in board
    static const int NUMCOLS = 15;       // numbers of columns in board

    // Width and height of a block of the board
    static const int BLOCK_WIDTH = WIDTH / NUMCOLS;
    static const int BLOCK_HEIGHT = HEIGHT / NUMROWS; 

    /* Color */
    int color[NUMROWS][NUMCOLS];    // it's [y][x] not [x][y]


public:
    Board();

};

#endif

