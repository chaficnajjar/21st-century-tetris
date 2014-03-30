
/*
 * Author: Chafic Najjar
 */

#ifndef TETROMINO_HPP
#define TETOMINO_HPP

// Put everything related to the tetromino like block coordinates, rotation, color, position, etc.
class Tetromino { 

public:
    static const int SIZE;
    int type;
    int color;

    // coords is a pointer to an array of unknown dimensions of int
    int (*coords)[2]; 

    static const int coordsTable[7][4][2];

    // coordinates of the block at (0, 0)
    int X, Y; 

    Tetromino(int type, int color);

    // Sets position of the block at (0, 0)
    void set_position(int x, int y);

    // Used by player to rotate tetrimino
    void rotate_left();

    // Counter-rotates tetromino when player rotates near a wall
    void rotate_right();
    
    // Get x coordinate of upper left of block i
    int get_block_x(int i);

    // Get y coordinate of upper left of block i
    int get_block_y(int i);

    void set_block_x(int i, int x);

    void set_block_y(int i, int y);

};

#endif
