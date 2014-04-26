//  Tetromino class declarations

#ifndef TETROMINO_HPP
#define TETROMINO_HPP

class Board;

class Tetromino { 

public:
    enum Status {INACTIVE, WAITING, FALLING, LANDED};
    enum Movement {NONE = 0, LEFT = -1, RIGHT = 1};
    static const int SIZE = 4;
    static const int coords_table[7][4][2];

public:
    Tetromino(int type);

    // Sets position of the block at (0, 0)
    void set_position(int new_x, int new_y) {x = new_x; y = new_y;}

    // Sets position of any block
    void set_block_x(int i, int new_x) {x = new_x - coords[i][0];}
    void set_block_y(int i, int new_y) {y = new_y - coords[i][1];}

    // Get x coordinate of upper left vertex of block i
    int get_block_x(int i) {return x + coords[i][0];}

    // Get y coordinate of upper left vertex of block i
    int get_block_y(int i) {return y + coords[i][1];}

    void add_to_x(int x_offset) { x += x_offset;}

    bool has_landed() {return status == LANDED;}
    void lands() {status = LANDED;}
    void drop() {status = FALLING;}

    void rotate_right();
    void rotate_left();

    void get_shadow(Board *board, int shadow_y[]);

public:
    Status status;
    Movement movement;
    int x, y;               // coordinates of the block at (0, 0)
    int type;
    bool free_fall;         // true if spacebar was pressed, falls down instantaneously
    bool speed_up;          // true if 's' or 'down' was pressed, falls down rapidly
    bool shift;             // true if player shifts tetromino left or right
    bool rotate;            // true if rotation occured (always counterclockwise)
    int (*coords)[2];       // relative coordinates (used for rotation)
};

#endif
