#include "input.h"
#include "stage.h"
#include "structs.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_scancode.h>

void do_key_down(App *app, Stage *stage, SDL_KeyboardEvent *event) {
	if (event->repeat > 0) {
		return;
	}

    key_pressed_stage(app, stage, event->keysym.scancode);
}

void do_mouse_down(App *app, Stage *stage, SDL_MouseButtonEvent *event) {
	mouse_click_stage(app, stage, event->x, event->y);
}

void input_do(App *app, Stage *stage) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			do_key_down(app, stage, &event.key);
			break;

		case SDL_MOUSEBUTTONDOWN:
			do_mouse_down(app, stage, &event.button);
			break;

		default:
			break;
		}
	}
}
