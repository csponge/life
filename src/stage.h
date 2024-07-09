#ifndef STAGE_H
#define STAGE_H

#include "structs.h"

Stage *init_stage(App *app, int rows, int cols);
void draw_stage(Stage *stage, SDL_Renderer *renderer);
void logic_stage(Stage *stage);
void mouse_click_stage(App *app, Stage *stage, int x, int y);
void free_stage(Stage *stage);

#endif
