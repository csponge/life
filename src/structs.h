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
    size_t num_elements;
    GuiElement **elements;
} Stage;

typedef struct {
	void (*logic)(App *app, Stage *stage);
	void (*draw)(App *app, Stage *stage);
} Delegate;

struct _App {
	bool run;
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Window *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOAD_KEYS];
};

#endif
