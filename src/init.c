#include "defs.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

App *init_app(void) {
	int render_flags, window_flags;

	render_flags = SDL_RENDERER_ACCELERATED;
	window_flags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	App *app = calloc(1, sizeof(App));
	if (app == NULL) {
		printf("Couldn't allocate app\n");
		return NULL;
	}

	app->window = SDL_CreateWindow("Life", SDL_WINDOWPOS_UNDEFINED,
	                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
	                               SCREEN_HEIGHT, window_flags);

	if (!app->window) {
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH,
		       SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app->renderer = SDL_CreateRenderer(app->window, -1, render_flags);

	if (!app->renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() != 0) {
		printf("Failed to initialize ttf library: %s\n",
		       SDL_GetError());
		exit(1);
	}

	app->font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 24);
	if (app->font == NULL) {
		printf("Failed to load TTF font: %s\n", SDL_GetError());
		exit(1);
	}

    int renderStyle = TTF_STYLE_NORMAL;
    TTF_SetFontStyle(app->font, renderStyle);
    TTF_SetFontHinting(app->font, TTF_HINTING_MONO);

	return app;
}

void free_app(App *app) {
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	TTF_CloseFont(app->font);
	free(app);
	SDL_Quit();
}
