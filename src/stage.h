#ifndef STAGE_H
#define STAGE_H

#include "structs.h"

Stage *init_stage(App *app, int rows, int cols);
void add_elem_to_stage(Stage *stage, GuiElement *element);
void free_stage(Stage *stage);

#endif
