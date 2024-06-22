#include "command.h"
#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "stage.h"
#include "structs.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>

static void cap_frame_rate(long *then, float *remainder) {
	long wait, frame_time;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frame_time = SDL_GetTicks64() - *then;

	wait -= frame_time;

	if (wait < 1) {
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}

int main() {
	long then;
	float remainder;

	App *app = init_app();

	int rows = SCREEN_HEIGHT / CELL;
	int cols = SCREEN_WIDTH / CELL;

	Stage *stage = init_stage(app, rows, cols);

	command_queue_init();

	then = SDL_GetTicks();
	remainder = 0;

	while (1) {
		prepare_scene(app);

		do_input(app, stage);

		Command cmd = command_dequeue();
		switch (cmd) {
		case Nop:
			break;
		case Play:
			app->run = true;
			break;
		}

        if (app->run == true) {
            app->delegate.logic(app, stage);
        }

		app->delegate.draw(app, stage);

		present_scene(app);

		cap_frame_rate(&then, &remainder);
	}

	// cleanup
	command_queue_free();
	free_stage(stage);
	free_app(app);
	return 0;
}
