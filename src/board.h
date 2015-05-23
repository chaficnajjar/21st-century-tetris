// Copyright [2015] <Chafic Najjar>

#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

class Board {
 public:
    static const int HEIGHT = 600;
    static const int WIDTH  = 300;
    static const int ROWS = 30;
    static const int COLS = 15;
    static const int BLOCK_HEIGHT = HEIGHT / ROWS;
    static const int BLOCK_WIDTH = WIDTH / COLS;
    static const int BONUS = 3;
    int color[ROWS][COLS];
    bool render_score;

    Board();
    void increase_score_by(int delta) {score += delta;}
    int get_score() {return score;}
    void delete_full_rows();
    bool add(Tetromino* tetro);

 private:
    bool full_row(int row);
    void shift_down(int row);
    int score;
};

#endif  // SRC_BOARD_H_
