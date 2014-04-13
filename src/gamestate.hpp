
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "game_engine.hpp"

class GameState {
public:
    virtual void init(GameEngine* game) = 0;
    virtual void clean_up(GameEngine* game) = 0;
    
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void input(GameEngine* game) = 0;
    virtual void update(GameEngine* game) = 0;
    virtual void render(GameEngine* game) = 0;

    void change_state(GameEngine* game, GameState* state) {
        game->change_state(state);
    }

protected:
    GameState() { }
};

#endif
