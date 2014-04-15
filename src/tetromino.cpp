//  Tetromino class definitions

#include "tetromino.hpp"

const int Tetromino::coords_table[7][4][2] = { 

    { { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } },      //type 0            Z-Block
                                                        //       |_|_|_
                                                        //         |_|_| 

    { { 1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } },       //type 1   |_|      J-Block
                                                        //         |_|
                                                        //       |_|_|

    { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },         //type 3 |_|_|      O-Block
                                                        //       |_|_|

    { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 } },        //type 4 |_|_|_|    T-Block
                                                        //         |_|

    { { 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } },        //type 5            S-Block
                                                        //         |_|_|
                                                        //       |_|_|

    { { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } },        //type 6 |_|        I-Block
                                                        //       |_|
                                                        //       |_|
                                                        //       |_|

    { { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } }       //type 7 |_|        L-Block
                                                        //       |_|_
                                                        //       |_|_|

};

Tetromino::Tetromino(int new_type) {
    type = new_type;
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
