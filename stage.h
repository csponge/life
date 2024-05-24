#ifndef STAGE_H
#define STAGE_H

#include "structs.h"

Stage *init_stage(App *app, int rows, int cols);
void free_stage(Stage *stage);

#endif
