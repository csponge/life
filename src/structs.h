#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct app_t App;

typedef struct stage_t {
	Button *play_btn;
	Button *pause_btn;
	Button *seed_btn;
	Button *dec_tick_btn;
	Button *inc_tick_btn;
	CellGrid *cell_grid;
} Stage;

typedef struct mouse_t {
    bool down;
    int x;
    int y;
} Mouse;

struct app_t {
	bool run;
	int logic_tick;
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Window *window;
};

#endif
