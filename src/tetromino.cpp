//  Tetromino class definitions

#include "tetromino.hpp"

const int Tetromino::SIZE = 4;

// All possible coordinates
// 2 is the number of possible state of a block (present, absent, ...) 
const int Tetromino::coordsTable[7][4][2] = { 

    { { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } },      //type 0  |_|       The Dog
                                                        //      |_|_|
                                                        //      |_| 

    { { 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } },        //type 1 |_|        The Dog inverted
                                                        //       |_|_|
                                                        //         |_|

    { { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },        //type 2 |_|        The Stick
                                                        //       |_|
                                                        //       |_|
                                                        //       |_|

    { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 } },        //type 3 |_|_|_|    The T
                                                        //         |_|


    { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },         //type 4 |_|_|      The Square
                                                        //       |_|_|

    { { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } },      //type 5 |_|_|      The L
                                                        //         |_|
                                                        //         |_|

    { { 1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } }        //type 6 |_|_|      The L inverted
                                                        //       |_|
                                                        //       |_|
};

Tetromino::Tetromino(int new_type, int new_color) {
    type = new_type;
    color = new_color;
    status = INACTIVE;

    // Movement
    xoffset = 0;

    // Tetromino coordinates
    coords = new int[4][2];

    // Define Tetromino coordinates
    // For each block, copy type coordinates
    for (int i = 0; i < 4 ; i++) {
        coords[i][0] = coordsTable[type][i][0];
        coords[i][1] = coordsTable[type][i][1];
    }
}


// Sets position of the block at (0, 0)
void Tetromino::set_position(int x, int y) {
    X = x;
    Y = y;
}

// Used by player to rotate tetrimino
void Tetromino::rotate_left() {
    // Loop through all blocks
    for(int i = 0; i < SIZE; i++) {
        int temp = coords[i][0];
        coords[i][0] = -coords[i][1];
        coords[i][1] = temp;
    }
}

// Counter-rotates tetromino when player rotates near a wall
void Tetromino::rotate_right() {
    // Loop through all blocks
    for(int i = 0; i < SIZE; i++) {
        int temp = coords[i][0];
        coords[i][0] = coords[i][1];
        coords[i][1] = -temp;
    }
}

// Get x coordinate of upper left vertex of block i
int Tetromino::get_block_x(int i) { 
    return X + coords[i][0];
}

// Get y coordinate of upper left vertex of block i
int Tetromino::get_block_y(int i) {
    return Y + coords[i][1];
}

void Tetromino::set_block_x(int i, int x) {
    X = x - coords[i][0];
}

void Tetromino::set_block_y(int i, int y) {
    Y = y - coords[i][1];
}

void Tetromino::add_to_x(int xoffset) {
    X += xoffset;
}

bool Tetromino::has_landed() {
    return status == LANDED;
}

void Tetromino::lands() {
    status = LANDED;
}

void Tetromino::drop() {
    status = FALLING;
}
