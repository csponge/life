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

	then = SDL_GetTicks();
	remainder = 0;

    int logic_ticker = 0;

	while (1) {
		prepare_scene(app);

		do_input(app, stage);

        if (app->run == true) {
            if (logic_ticker == app->logic_tick || app->logic_tick == 0) {
                app->delegate.logic(app, stage);
                logic_ticker = 0;
            }
            logic_ticker++;
        }

		app->delegate.draw(stage, app->renderer);

		present_scene(app);

		cap_frame_rate(&then, &remainder);
	}

	// cleanup
	free_stage(stage);
	free_app(app);
	return 0;
}
