#include "input.h"
#include "stage.h"
#include "structs.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_scancode.h>

void do_key_down(App *app, SDL_KeyboardEvent *event) {
	/*if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOAD_KEYS)
	 * {*/
	/*  app->keyboard[event->keysym.scancode] = 1;*/
	/*}*/
	if (event->keysym.scancode == SDL_SCANCODE_R) {
		app->run = true;
	}
}

void do_key_up(App *app, SDL_KeyboardEvent *event) {
	/*if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOAD_KEYS)
	 * {*/
	/*  app->keyboard[event->keysym.scancode] = 0;*/
	/*}*/
}

void do_mouse_down(App *app, Stage *stage, SDL_MouseButtonEvent *event) {
    mouse_click_stage(app, stage, event->x, event->y);
}

void do_input(App *app, Stage *stage) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			do_key_down(app, &event.key);
			break;

		case SDL_KEYUP:
			do_key_up(app, &event.key);
			break;

		case SDL_MOUSEBUTTONDOWN:
			do_mouse_down(app, stage, &event.button);
			break;

		default:
			break;
		}
	}
}
