#ifndef STAGE_H
#define STAGE_H

#include "structs.h"
#include <SDL2/SDL_scancode.h>

Stage *init_stage(App *app, int rows, int cols);
void draw_stage(Stage *stage, SDL_Renderer *renderer);
void logic_stage(Stage *stage);
void key_pressed_stage(App *app, Stage *stage, SDL_Scancode code);
void mouse_click_stage(App *app, Stage *stage, int x, int y);
void free_stage(Stage *stage);

#endif
