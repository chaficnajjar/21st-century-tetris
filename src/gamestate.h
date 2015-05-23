// Copyright [2015] <Chafic Najjar>

#ifndef SRC_GAMESTATE_H_
#define SRC_GAMESTATE_H_

#include "src/game_engine.h"

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

#endif  // SRC_GAMESTATE_H_
