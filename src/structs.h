#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct _App App;

typedef struct {
    Button *play_btn;
    Button *pause_btn;
    Button *seed_btn;
    Button *dec_tick_btn;
    Button *inc_tick_btn;
    CellGrid *cell_grid;
} Stage;

typedef struct {
	void (*logic)(App *app, Stage *stage);
	void (*draw)(Stage *stage, SDL_Renderer *renderer);
} Delegate;

struct _App {
	bool run;
    int logic_tick;
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Window *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOAD_KEYS];
};

#endif
