
#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.hpp"

class MenuState : public GameState {
public:
    void init(GameEngine* game);
    void clean_up(GameEngine* game);

    void pause();
    void resume();
    void reset();

    void input(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static MenuState* Instance() { return &m_menustate; }

protected:
    MenuState() { }

private:
    static MenuState m_menustate;

    bool exit;
    bool play;

    // Fonts
    SDL_Color       white;
    SDL_Texture*    font_image_tetris;         
};

#endif
