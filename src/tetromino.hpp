//  Tetromino class declarations

#ifndef TETROMINO_HPP
#define TETROMINO_HPP

// Put everything related to the tetromino like block coordinates, rotation, color, position, etc.
class Tetromino { 

public:
    static const int SIZE;
    int type;
    int color;

    enum Status {INACTIVE, WAITING, FALLING, LANDED};
    Status status;

    // Movement
    int xoffset;

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

    void add_to_x(int xoffset);

    bool has_landed();

    void lands();

    void drop();

};

#endif
