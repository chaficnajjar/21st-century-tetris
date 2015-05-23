// Copyright [2015] <Chafic Najjar>

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <string>

void render_texture(SDL_Texture *tex,
        SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);
void render_texture(SDL_Texture *tex,
        SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);
SDL_Texture* render_text(const std::string &message,
        SDL_Color color, TTF_Font* font, SDL_Renderer *renderer);
SDL_Texture* load_texture(const std::string &file, SDL_Renderer *ren);

#endif  // SRC_UTILITIES_H_
