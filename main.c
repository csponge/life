#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "structs.h"
#include "stage.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 10
#define COLS 20

#define clear() printf("\033[H\033[J")

void display(char **grid) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      printf("%c", grid[row][col]);
    }
    printf("\n");
  }
}

char **generate_seed(int rows, int cols) {
  char **seed = calloc(rows, sizeof(char *));
  for (int row = 0; row < rows; row++) {
    seed[row] = calloc(cols, sizeof(char));
  }

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int isAlive = rand() % (10 + 1);

      if (isAlive <= 3) {
        seed[row][col] = 'O';
      } else {
        seed[row][col] = ' ';
      }
    }
  }

  return seed;
}

int count_live_neighbors(char **grid, int row, int col) {
  int live_neighbors = 8;

  // top
  if ((row > 0 && col > 0 ? grid[row - 1][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((row > 0 ? grid[row - 1][col] : ' ') == ' ')
    live_neighbors--;

  if ((row > 0 && col < (COLS - 1) ? grid[row - 1][col + 1] : ' ') == ' ')
    live_neighbors--;

  // middle
  if ((col > 0 ? grid[row][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((col < (COLS - 1) ? grid[row][col + 1] : ' ') == ' ')
    live_neighbors--;

  // bottom
  if ((row < (ROWS - 1) && col > 0 ? grid[row + 1][col - 1] : ' ') == ' ')
    live_neighbors--;

  if ((row < (ROWS - 1) ? grid[row + 1][col] : ' ') == ' ')
    live_neighbors--;

  if ((row < (ROWS - 1) && col < (COLS - 1) ? grid[row + 1][col + 1] : ' ') ==
      ' ')
    live_neighbors--;

  return live_neighbors;
}

void evaluate(char **grid) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int live_neighbors = count_live_neighbors(grid, row, col);

      // handle dead cells
      if (grid[row][col] == ' ') {
        if (live_neighbors == 3) {
          grid[row][col] = 'O';
        }
        continue;
      }

      // handle alive cells
      if (live_neighbors < 2 || live_neighbors > 3) {
        grid[row][col] = ' ';
      }
    }
  }
}

static void cap_frame_rate(long *then, float *remainder) {
  long wait, frame_time;

  wait = 16 + *remainder;

  *remainder -= (int)*remainder;

  frame_time = SDL_GetTicks() - *then;

  wait -= frame_time;

  if (wait < 1) {
    wait = 1;
  }

  SDL_Delay(wait);

  *remainder += 0.667;

  *then = SDL_GetTicks();
}

int main() {
  // generate seed
  // char **seed = generate_seed(ROWS, COLS);

  //// main loop
  // while (1) {
  //   evaluate(seed);
  //   display(seed);
  //   sleep(1);
  //   clear();
  // }

  //// cleanup
  // for (int row = 0; row < ROWS; row++) {
  //   free(seed[row]);
  // }
  // free(seed);

  long then;
  float remainder;

  App app = init_sdl();

  Entity player;

  /* Entity bullet = { */
  /*   .texture = load_texture(&app, "gfx/bullet.png"), */
  /* }; */

  SDL_Texture *bullet_texture = load_texture(&app, "gfx/bullet.png");

  Stage *stage = init_stage(&app, &player);

  /* get_texture_rect(bullet.texture, &bullet.w, &bullet.h); */

  then = SDL_GetTicks();
  remainder = 0;

  while (1) {
    prepare_scene(&app);

    do_input(&app);

    app.delegate.logic(&app, stage, &player, bullet_texture);
    app.delegate.draw(&app, stage, &player);

    present_scene(&app);

    cap_frame_rate(&then, &remainder);
  }

  return 0;
}
