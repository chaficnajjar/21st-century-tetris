
/*
 * Author: Chafic Najjar
 * Tetris game written in C++ using SDL 2.0
 */

#include "tetris.hpp"
#include "tetromino.hpp"
#include "board.hpp"
#include "utilities.hpp"

#include <random>

std::random_device rd;
std::mt19937 gen(rd());

const float Tetris::colors[NCOLORS][4] = {
    { 255, 0  , 0  , 255 },      // red
    { 0  , 255, 0  , 255 },      // blue
    { 0  , 0  , 255, 255 },      // turquoise
    { 255, 255, 0  , 255 },      // purple
    { 0  , 255, 255, 255 },      // brown
    { 255, 0  , 255, 255 }       // yellow
};

Tetris::Tetris(int argc, char *argv[]) {

    /* Initialize audio, CD-ROM, event handling, file I/O, joystick handling, threading, timers and videos */
    SDL_Init(SDL_INIT_EVERYTHING);

    /* Window and renderer */
    window = SDL_CreateWindow( "Tetris Unleashed!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN );

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    /* Game objects */
    board        = new Board();
    tetro        = new Tetromino( rand()%7, rand()%NCOLORS );       // current tetromino
    next_tetro   = new Tetromino( rand()%7, rand()%NCOLORS );       // next tetromino

    /* Fonts */
    TTF_Init();
    white = { 255, 255, 255 };

    font_image_tetris = renderText("Tetris Unleashed!", "resources/fonts/bitwise.ttf", white, 16, renderer);
    font_image_score_text = renderText("Score: ", "resources/fonts/bitwise.ttf", white, 20, renderer);
    font_image_score = renderText(std::to_string(board->get_score()), "resources/fonts/bitwise.ttf", white, 20, renderer);
    font_image_new_game = renderText("New game", "resources/fonts/bitwise.ttf", white, 20, renderer);
    font_image_quit = renderText("Quit", "resources/fonts/bitwise.ttf", white, 20, renderer);
    font_image_game_over = renderText("Game over!", "resources/fonts/bitwise.ttf", white, 16, renderer);

    x_offset         = 0;
    rotate_left     = false;
    shifted         = false;
    free_fall       = false;
    speed_up         = false;

    newgamedown     = false;
    newgameup       = false;
    quitdown        = false;
    quitup          = false;

    game_over       = false;
    done            = false;

    gameoffset    = 20.0f;

    acceleration  = 0.005f;

    this_time      = 0;
    last_time      = 0;

    time_till_drop= 0.3f;
    time_counter  = 0.0f;

    // Coordinates of the "New Game" button
    // Also used for "Quit" button
    newgamex1 = gameoffset+board->WIDTH+board->BLOCK_WIDTH;       // left
    newgamex2 = gameoffset+board->WIDTH+8*board->BLOCK_WIDTH;     // right
    newgamey1 = board->HEIGHT-4*board->BLOCK_HEIGHT;              // down
    newgamey2 = board->HEIGHT-6*board->BLOCK_HEIGHT;              // up


    // At the start of the game:
    // x position of (0, 0) block of tetro is int(15/2) = 7 which is the exact horizontal middle of board
    // y position of (0, 0) block of tetro is 0 which is the top of the board
    tetro->set_position(int(board->NUMCOLS/2), 0);

    // Position next_tetro at the upper right of the window, outside of the board
    next_tetro->set_position(board->NUMCOLS+5,int(0.3*board->NUMROWS)); 

}
 
// Get player input
void Tetris::input() {

    // Queuing events
    SDL_Event event;
    while (SDL_PollEvent(&event)) { 

        // Clicking 'x' or pressing F4
        if (event.type == SDL_QUIT) 
            done = true;

        // Key is pressed
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    done = true; 
                    break;
                case SDLK_a: case SDLK_LEFT:
                    x_offset = -1;
                    shifted = true;
                    break;
                case SDLK_d: case SDLK_RIGHT:
                    x_offset = 1;
                    shifted = true;
                    break;
                case SDLK_w: case SDLK_UP: 
                    if(tetro->type != 4)        // type 4 is the square
                        rotate_left = true;
                    break;
                case SDLK_s: case SDLK_DOWN:
                    speed_up = true;
                    break;
                case SDLK_SPACE:
                    free_fall = true; 
                    break;
                default: 
                    break;
            }

        }

        // Key is released
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_s: case SDLK_DOWN:
                    speed_up = false;
                    break;
                default:
                    break;
            } 
        }

        // Mouse moves
        if (event.type == SDL_MOUSEMOTION) {

            // Outside of the board
            if (event.motion.x > board->WIDTH + gameoffset) 
                SDL_ShowCursor(1);          // show cursor
            
            // Inside the board
            else
                SDL_ShowCursor(0);          // don't show cursor
        }

        // Mouse button clicked
        if (event.type == SDL_MOUSEBUTTONDOWN)
            switch (event.button.button) {

                // Left mouse button clicked
                case SDL_BUTTON_LEFT:
                    if (event.button.x > newgamex1 && event.button.x < newgamex2) {

                        // And mouse cursor is on "New Game" button 
                        if (event.button.y > newgamey2 && event.button.y < newgamey1)
                            newgamedown = true;

                        // And mouse cursor is on "Quit" button
                        else if (event.button.y > newgamey2+4*board->BLOCK_HEIGHT && event.button.y < newgamey1+4*board->BLOCK_HEIGHT)
                            quitdown = true;
                    }
                    break;
                default:
                    break;
            }

        // Mouse button released 
        if (event.type == SDL_MOUSEBUTTONUP)
            switch (event.button.button) {

                // Left mouse button released
                case SDL_BUTTON_LEFT:
                    if (event.button.x > newgamex1 && event.button.x < newgamex2) {

                        // And mouse cursor is on "New Game" button
                        if (event.button.y > newgamey2 && event.button.y < newgamey1)
                            newgameup = true;

                        // And mouse cursor is on "Quit" button
                        else if (event.button.y > newgamey2+4*board->BLOCK_HEIGHT && event.button.y < newgamey1+4*board->BLOCK_HEIGHT)
                            quitup = true;
                    }
                    break;
                default:
                    break;
            }
    }
}

void Tetris::release_tetromino() {
    Tetromino *new_tetro = new Tetromino(rand()%7, rand()%NCOLORS); // assign random type and color to new tetromino
    new_tetro->set_position(next_tetro->X, next_tetro->Y);          // new tetrominoe is positioned under "Next Piece"

    delete [] tetro;                                // delete allocated memory
    tetro = next_tetro;                             // update falling tetromino
    tetro->set_position(int(board->NUMCOLS/2), 0);  // position the newly falling tetromino

    next_tetro = new_tetro;                         // update next_tetro to point to tetromino under "Next Piece"

    tetro->drop();                                  // old falling tetromino has landed and new tetromino has already been generated
    
}

// Update game values
void Tetris::update() { 

    //=== Tetromino has crossed over the top border of the board ===//
    if (game_over)
        return;

    //=== Tetromino has landed ===//
    if (tetro->has_landed()) {
        free_fall = false;  

        // Add fallen tetromino to the board and check if tetromino has crossed over the top border
        if (!board->add(tetro)) {
            game_over = true;
            return;
        }

        // Drop stored tetromino and replace by newly-generated tetromino
        release_tetromino();

    }

    //=== Free fall ===//
    else if (free_fall)
        tetro->Y++;     // maximum speed
                        // no rotations or translations allowed during free fall

    //=== Rotations and translations ===//
    else {

        // Rotation
        if (rotate_left)
            tetro->rotate_left();

        // Update tetromino position on the x-axis
        tetro->add_to_x(x_offset);

        // Assign the time required for tetromino to fall down one block
        if (speed_up)
            time_till_drop = 0.02f; // 2x slower than free fall

        else
            time_till_drop = 0.3f - board->get_score()*acceleration;    // drop speed proportional to score

        // Add time elapsed
        time_counter += frame_rate(&last_time, &this_time);

        // time_till_drop = 0.3;
        // delta_time ~ 0.017 seconds
        // Tetromino initially falls one block unit for every 0.3/0.017 ~ 17 game loops

        if (time_counter >= time_till_drop) { 
            tetro->Y++;     // update tetromino position on the y-axis
            time_counter = 0.0f;
        }

    }

    //=== Collision detection ===//
    // Check if tetromino is in an acceptable position. If not, undo previous move(s)
    for (int i = 0; i < tetro->SIZE; i++) {

        // Coordinates of each block
        int x = tetro->get_block_x(i);
        int y = tetro->get_block_y(i); 

        // Block crosses wall after rotation and/or translation 
        if (x < 0 || x >= board->NUMCOLS) {

            // Because of rotation
            if (rotate_left) 
                tetro->rotate_right();       // neutralize the left rotation 

            // Because of translation
            if (shifted) 
                tetro->X -= x_offset;        // neutralize shift
        }

        // Block touches ground
        if (y >= board->NUMROWS) {
            tetro->lands();
            tetro->set_block_y(i, board->NUMROWS-1); // change the value of Y so that block(s) of the (old) tetromino is/are above the blue line
        }

        // Block is on the board
        else if (y >= 0) 

            // Block touched another block
            if (board->color[y][x] != -1) {

                // Tetromino rotates and collides with a block
                if (rotate_left || shifted) {
                    if (rotate_left)
                        tetro->rotate_right();          // neutralize
                    
                    // Tetromino is shifted into another block
                    if (shifted)
                        tetro->X -= x_offset;           // neutralize
                }

                // Block falls into another block
                else {
                    tetro->Y--;     // neutralize: tetromino goes up
                    tetro->lands();
                }

            }
    }

    //=== Delete full rows ===//
    board->delete_rows();

    // Done rotating and shifting
    rotate_left = false; 
    shifted = false;
    x_offset = 0;


}

// Create "New Game" and "Quit" buttons
void Tetris::create_button(int x, int y, int width, int height, int k) {

        SDL_Rect rect = { x, y, width, height };
        SDL_SetRenderDrawColor(renderer, colors[k][0], colors[k][1], colors[k][2], colors[k][3]);
        SDL_RenderFillRect(renderer, &rect);

}

// Render Tetromino block
void Tetris::draw_block(int x, int y, int k) {

    SDL_Rect block = {x, y, int(board->BLOCK_WIDTH), int(board->BLOCK_HEIGHT)};
    SDL_SetRenderDrawColor(renderer, colors[k][0], colors[k][1], colors[k][2], colors[k][3]);
    SDL_RenderFillRect(renderer, &block);

}

// Render result
void Tetris::render() {

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);


    int x = (next_tetro->X-3)*board->BLOCK_WIDTH; 
    int y = gameoffset; 

    renderTexture(font_image_tetris, renderer, x, y);

    // Render score text
    renderTexture(font_image_score_text, renderer, x, y + board->BLOCK_WIDTH);

    // Render score
    if (board->render_score) {
        font_image_score = renderText(std::to_string(board->get_score()), "resources/fonts/bitwise.ttf", white, 20, renderer);
        board->render_score = false;
    }
    renderTexture(font_image_score, renderer, x + 60, y + board->BLOCK_WIDTH);
    
    int tetro_x, tetro_y;

    // Draw tetromino squares
    for (int i = 0; i < tetro->SIZE; i++) {

        // Get new coordinates
        tetro_x = float(tetro->get_block_x(i))*board->BLOCK_WIDTH + gameoffset;
        tetro_y = float(tetro->get_block_y(i))*board->BLOCK_HEIGHT + gameoffset;

        draw_block(tetro_x, tetro_y, tetro->color);

    }

    if (!game_over)

        // Draw next tetromino
        for (int i = 0; i < next_tetro->SIZE; i++) {


            // Get new coordinates
            tetro_x = float(next_tetro->get_block_x(i))*board->BLOCK_WIDTH;
            tetro_y = float(next_tetro->get_block_y(i))*board->BLOCK_HEIGHT;

            draw_block(tetro_x, tetro_y, next_tetro->color);

        }

    // This is the board. Non-active tetrominos live here.
    for (int i = 0; i < board->NUMROWS; i++)
        for (int j = 0; j < board->NUMCOLS; j++)

            if (board->color[i][j]!=-1) {


                // Get new coordinates
                tetro_x = float(j)*board->BLOCK_WIDTH + gameoffset;
                tetro_y = float(i)*board->BLOCK_HEIGHT + gameoffset;

                draw_block(tetro_x, tetro_y, board->color[i][j]);

            }

    
    // Box surrounding board

    // Set color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Draw left border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset, gameoffset, gameoffset+board->HEIGHT);

    // Draw right border
    SDL_RenderDrawLine(renderer, gameoffset+board->WIDTH, gameoffset, gameoffset+board->WIDTH, gameoffset+board->HEIGHT);

    // Draw upper border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset, gameoffset+board->WIDTH, gameoffset);

    // Draw bottom border
    SDL_RenderDrawLine(renderer, gameoffset, gameoffset+board->HEIGHT, gameoffset+board->WIDTH, gameoffset+board->HEIGHT);

    // If game is over, display "Game Over!"
    if (game_over)
        renderTexture(font_image_game_over, renderer, newgamex1, SCREEN_HEIGHT-newgamey1+4*board->BLOCK_WIDTH);

    // Create "New Game" button
    create_button(newgamex1, newgamey2, 7*board->BLOCK_WIDTH, 2*board->BLOCK_HEIGHT, 2);

    // Render "New Game" font
    renderTexture(font_image_new_game, renderer, newgamex1+10, newgamey2+10);

    // Create "Quit" button
    create_button(newgamex1, newgamey2+4*board->BLOCK_HEIGHT, 7*board->BLOCK_WIDTH, 2*board->BLOCK_HEIGHT, 0);

    // Render "Quit" font
    renderTexture(font_image_quit, renderer, newgamex1+10, newgamey2+4*board->BLOCK_HEIGHT+10);

    // Swap buffers
    SDL_RenderPresent(renderer);

}

// Restarts game
void Tetris::reset() { 

    for(int i = 0; i < board->NUMROWS; i++)
        for(int j = 0; j < board->NUMCOLS; j++)
            board->color[i][j] = -1; 

    // release allocated memory
    delete [] board;
    delete [] tetro;
    delete [] next_tetro;

    // recreate objects
    board = new Board();
    tetro = new Tetromino(rand()%7, rand()%NCOLORS ); 
    next_tetro = new Tetromino(rand()%7, rand()%NCOLORS );

    tetro->set_position(int(board->NUMCOLS/2),0);
    next_tetro->set_position(board->NUMCOLS+5,int(0.3*board->NUMROWS));

    rotate_left         = false;
    shifted         = false;
    free_fall        = false;
    
    launch_tetro        = false;
    game_over        = false;
    newgameup       = false;
    newgamedown     = false;

}

void Tetris::clean_up() {
    SDL_DestroyTexture(font_image_tetris);
    SDL_DestroyTexture(font_image_score_text);
    SDL_DestroyTexture(font_image_score);
    SDL_DestroyTexture(font_image_new_game);
    SDL_DestroyTexture(font_image_quit);
    SDL_DestroyTexture(font_image_game_over);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Execute game loop
void Tetris::execute() {

    do {

        input();
        update();
        render();

        if(newgameup && newgamedown)
            reset();             

        if(quitup && quitdown)
            done = true;

    } while(!done);

    clean_up();
}

float Tetris::frame_rate(int *last_time, int *this_time) {

    // Get number of milliseconds since SDL_Init() of the previous frame
    *last_time = *this_time; 

    // Get number of milliseconds since SDL_Init()
    *this_time = SDL_GetTicks(); 

    // Variation of time between each game iteration
    return ((*this_time - *last_time) / 1000.0f);         // dividing by 1000 to convert in seconds

}

