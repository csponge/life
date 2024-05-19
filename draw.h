#ifndef DRAW_H
#define DRAW_H

#include "structs.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

void prepare_scene(App *app);
void present_scene(App *app);
SDL_Texture *load_texture(App *app, char *filename);
void get_texture_rect(SDL_Texture *texture, int *w, int *h);
void blit(App *app, SDL_Texture *texture, int x, int y);

#endif
