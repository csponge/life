#include "input.h"
#include "structs.h"
#include <SDL2/SDL_events.h>
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

void do_mouse_down(App *app, SDL_MouseButtonEvent *event) {
	app->mouse.clicked = true;
	app->mouse.x = event->x;
	app->mouse.y = event->y;
}

void do_mouse_up(App *app, SDL_MouseButtonEvent *event) {
	app->mouse.clicked = false;
	app->mouse.x = -1;
	app->mouse.y = -1;
}

void do_input(App *app) {
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
			do_mouse_down(app, &event.button);
			break;

		case SDL_MOUSEBUTTONUP:
			do_mouse_up(app, &event.button);
			break;
		default:
			break;
		}
	}
}
