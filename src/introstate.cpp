
#include "introstate.hpp"
#include "playstate.hpp"
#include "utilities.hpp"

IntroState IntroState::m_introstate;

void IntroState::init(GameEngine* game) {
    logo = load_texture("resources/images/logo.png", game->renderer);

    exit = false;
    play = false;
}

void IntroState::clean_up(GameEngine* game) {
    IMG_Quit();
}

void IntroState::pause() {

}

void IntroState::resume() {

}

void IntroState::reset() {

}

void IntroState::input(GameEngine* game) {
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

void IntroState::update(GameEngine* game) {
    if (play)
        game->push_state(PlayState::Instance());
}

void IntroState::render(GameEngine* game) {
    // Clear screen
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    int iW, iH;
    SDL_QueryTexture(logo, NULL, NULL, &iW, &iH);
    int x = game->SCREEN_WIDTH / 2 - iW / 2;
    int y = game->SCREEN_HEIGHT / 2 - iH / 2;
    render_texture(logo, game->renderer, x, y);

    // Swap buffers
    SDL_RenderPresent(game->renderer);

}
