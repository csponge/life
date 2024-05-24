#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "structs.h"
#include "stage.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
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

  printf("Running with rows:%d cols:%d\n", rows, cols);

  Stage *stage = init_stage(app, rows, cols);

  then = SDL_GetTicks();
  remainder = 0;

  while (1) {

    prepare_scene(app);

    do_input(app);
    
    app->delegate.mouse_click(app, stage);
    app->delegate.logic(app, stage);
    app->delegate.draw(app, stage);

    present_scene(app);

    cap_frame_rate(&then, &remainder);
  }

  // cleanup
  free_stage(stage);
  free_app(app);
  return 0;
}
