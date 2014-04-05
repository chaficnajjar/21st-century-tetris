//  Tetromino class definitions

#include "tetromino.hpp"

const int Tetromino::coords_table[7][4][2] = { 

    { { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } },      //type 0   |_|      The Dog
                                                        //       |_|_|
                                                        //       |_| 

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
    free_fall = false;
    speed_up = false;
    status = INACTIVE;
    movement = NONE;
    coords = new int[4][2];

    for (int i = 0; i < 4 ; i++) {
        coords[i][0] = coords_table[type][i][0];
        coords[i][1] = coords_table[type][i][1];
    }
}

void Tetromino::rotate_left() {
    for (int i = 0; i < SIZE; i++) {
        int temp = coords[i][0];
        coords[i][0] = -coords[i][1];
        coords[i][1] = temp;
    }
}

void Tetromino::rotate_right() {
    for (int i = 0; i < SIZE; i++) {
        int temp = coords[i][0];
        coords[i][0] = coords[i][1];
        coords[i][1] = -temp;
    }
}
