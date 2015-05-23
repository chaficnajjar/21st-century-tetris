// Copyright [2015] <Chafic Najjar>

#include "src/utilities.h"

void render_texture(SDL_Texture *tex,
        SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip) {
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void render_texture(SDL_Texture* tex,
        SDL_Renderer *ren, int x, int y, SDL_Rect* clip) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    }

    render_texture(tex, ren, dst, clip);
}

SDL_Texture* render_text(const std::string &message,
        SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    return texture;
}

SDL_Texture* load_texture(const std::string &file, SDL_Renderer* ren) {
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
    return texture;
}
