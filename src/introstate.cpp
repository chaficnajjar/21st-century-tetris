
#include "introstate.hpp"
#include "menustate.hpp"
#include "utilities.hpp"

IntroState IntroState::m_introstate;

void IntroState::init(GameEngine* game) {
    logo = load_texture("resources/images/logo.png", game->renderer);

    exit = false;
    alpha = 1;
    logo_status = FADE_IN;
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
                default: 
                    break;
            }
        }
    }
}


#include <iostream>
void IntroState::update(GameEngine* game) {
    if (exit)
        game->quit();

    if (alpha == 0)
        game->push_state(MenuState::Instance());
}

void IntroState::render(GameEngine* game) {
    // Clear screen
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 1);
    SDL_RenderClear(game->renderer);

    int iW, iH;
    SDL_QueryTexture(logo, NULL, NULL, &iW, &iH);
    int x = game->SCREEN_WIDTH / 2 - iW / 2;
    int y = game->SCREEN_HEIGHT / 2 - iH / 2;

    if (logo_status == FADE_IN) {
        alpha += 3;
        if (alpha >= 255) {
            alpha = 255;
            logo_status = REMAIN;
        }
    }

    else if (logo_status == REMAIN) {
        SDL_Delay(2000);
        logo_status = FADE_OUT;
    }

    else if (logo_status == FADE_OUT) {
        alpha -= 3;
        if (alpha <= 0) {
            alpha = 0;
            logo_status = FADE_IN; 
        }
    }

    SDL_SetTextureAlphaMod(logo, alpha);

    render_texture(logo, game->renderer, x, y);

    // Swap buffers
    SDL_RenderPresent(game->renderer);

}
