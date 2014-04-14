
#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gamestate.hpp"

class IntroState : public GameState {
public:
    void init(GameEngine* game);
    void clean_up(GameEngine* game);

    void pause();
    void resume();
    void reset();

    void input(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static IntroState* Instance() { return &m_introstate; }

protected:
    IntroState() { }

private:
    static IntroState m_introstate;

    bool exit;

    // Logo
    SDL_Texture* logo;
    int alpha;
    enum Status {FADE_IN, REMAIN, FADE_OUT};
    Status logo_status;
};

#endif
