#ifndef STAGE_H
#define STAGE_H

#include "structs.h"

Stage *init_stage(App *app, int rows, int cols);
void mouse_click(Stage *stage, int x, int y);
void free_stage(Stage *stage);

#endif
