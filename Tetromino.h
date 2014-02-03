
/*
 * Author: Chafic Najjar
 * NOTE: The origin of the coordinate system is the upper-left point of the window
 */

// Put everything related to the tetromino like block coordinates, rotation, color, position, etc.
class Tetromino { 

public:
    static const int SIZE = 4;
    int type;
    int color;

    // coords is a pointer to an array of unknown dimensions of int
    // http://stackoverflow.com/questions/10350461/c-asterisk-and-bracket-operators-used-together
    int (*coords)[2]; 

    // coordinates of the block at (0, 0)
    int X, Y; 

    Tetromino(int type, int color) {

        // Tetromino coordinates
        coords = new int[4][2];

        // All possible coordinates
        // 2 is the number of possible state of a block (present, absent, ...) 
        static const int coordsTable[7][4][2] = { 

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


        // Define Tetromino coordinates
        // For each block, copy type coordinates
        for (int i = 0; i < 4 ; i++) {
            coords[i][0] = coordsTable[type][i][0];
            coords[i][1] = coordsTable[type][i][1];
        }

        // Set type and color variables
        this->type = type;
        this->color = color;

    }

    // Sets position of the block at (0, 0)
    void SetPosition(int x, int y) {
        X = x;
        Y = y;
    }

    // Used by player to rotate tetrimino
    void RotateLeft() {

        // Loop through all blocks
        for(int i = 0; i < SIZE; i++) {
            int temp = coords[i][0];
            coords[i][0] = -coords[i][1];
            coords[i][1] = temp;
        }
    }

    // Counter-rotates tetromino when player rotates near a wall
    void RotateRight() {

        // Loop through all blocks
        for(int i = 0; i < SIZE; i++) {
            int temp = coords[i][0];
            coords[i][0] = coords[i][1];
            coords[i][1] = -temp;
        }
    }
    
    // Get x coordinate of upper left of block i
    int GetBlockX(int i) { 
        return X + coords[i][0];
    }

    // Get y coordinate of upper left of block i
    int GetBlockY(int i) {
        return Y + coords[i][1];
    }

    void SetBlockX(int i, int x) {
        X = x - coords[i][0];
    }

    void SetBlockY(int i, int y) {
        Y = y - coords[i][1];
    }

};
