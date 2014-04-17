
#include "menustate.hpp"
#include "playstate.hpp"
#include "utilities.hpp"

MenuState MenuState::m_menustate;

void MenuState::init(GameEngine* game) {
    exit = false;
    play = false;

    // Fonts
    TTF_Init();
    white = { 255, 255, 255 };
    font_image_press_start = render_text("Press start!", "resources/fonts/bitwise.ttf", white, 16, game->renderer);
}

void MenuState::clean_up(GameEngine* game) {
    SDL_DestroyTexture(font_image_press_start);
    IMG_Quit();
}

void MenuState::pause() {

}

void MenuState::resume() {

}

void MenuState::reset() {

}

void MenuState::input(GameEngine* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) { 

        // Clicking 'x' or pressing F4
        if (event.type == SDL_QUIT) 
            exit = true;

        // Key is pressed
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    exit = true; 
                    break;
                case SDLK_SPACE:
                    play = true; 
                    break;
                default: 
                    break;
            }
        }
    }
}

void MenuState::update(GameEngine* game) {
    if (exit)
        game->quit();

    if (play)
        game->push_state(PlayState::Instance());
}

void MenuState::render(GameEngine* game) {
    // Clear screen
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    int height, width;
    SDL_QueryTexture(font_image_press_start, nullptr, nullptr, &width, &height);

    render_texture(font_image_press_start, game->renderer, (game->width - width)/2, (game->height - height)/2); 
    // Swap buffers
    SDL_RenderPresent(game->renderer);
}
