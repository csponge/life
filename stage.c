#include "structs.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdlib.h>

int count_live_neighbors(Stage *stage, int row, int col) {
  int live_neighbors = 8;

  // top
  if ((row > 0 && col > 0 ? stage->cells[row - 1][col - 1]->alive : false) ==
      false)
    live_neighbors--;

  if ((row > 0 ? stage->cells[row - 1][col]->alive : false) == false)
    live_neighbors--;

  if ((row > 0 && col < (stage->cols - 1)
           ? stage->cells[row - 1][col + 1]->alive
           : false) == false)
    live_neighbors--;

  // middle
  if ((col > 0 ? stage->cells[row][col - 1]->alive : false) == false)
    live_neighbors--;

  if ((col < (stage->cols - 1) ? stage->cells[row][col + 1]->alive : false) ==
      false)
    live_neighbors--;

  // bottom
  if ((row < (stage->rows - 1) && col > 0
           ? stage->cells[row + 1][col - 1]->alive
           : false) == false)
    live_neighbors--;

  if ((row < (stage->rows - 1) ? stage->cells[row + 1][col]->alive : false) ==
      false)
    live_neighbors--;

  if ((row < (stage->rows - 1) && col < (stage->cols - 1)
           ? stage->cells[row + 1][col + 1]->alive
           : false) == false)
    live_neighbors--;

  return live_neighbors;
}

void logic(Stage *stage) {
  for (int row = 0; row < stage->rows; row++) {
    for (int col = 0; col < stage->cols; col++) {
      int live_neighbors = count_live_neighbors(stage, row, col);

      // handle dead cells
      if (stage->cells[row][col]->alive == false) {
        if (live_neighbors == 3) {
          stage->cells[row][col]->alive = true;
        }
        continue;
      }

      // handle alive cells
      if (live_neighbors < 2 || live_neighbors > 3) {
        stage->cells[row][col]->alive = false;
      }
    }
  }
}

void draw_cell(App *app, Cell *cell, int w, int h) {
  SDL_Rect dest;

  dest.x = cell->x;
  dest.y = cell->y;
  dest.w = w;
  dest.h = h;

  // set the render color for all cells
  if (SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "failed to set the render color: %s", SDL_GetError());
  }

  if (SDL_RenderDrawRect(app->renderer, &dest) != 0) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "failed to draw rectangle: %s", SDL_GetError());
  }

  // only alive cells should be filled in
  if (cell->alive == true) {
    if (SDL_RenderFillRect(app->renderer, &dest) != 0) {
      SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                     "failed to color rectangle: %s", SDL_GetError());
    }
  }
}

void draw_cells(App *app, Stage *stage) {
  int x = 0;
  int y = 0;

  for (int row = 0; row < stage->rows; row++) {
    for (int col = 0; col < stage->cols; col++) {
      Cell *cell = stage->cells[row][col];
      cell->x = x;
      cell->y = y;

      draw_cell(app, cell, stage->cell_h, stage->cell_w);

      x += stage->cell_w;
    }
    y += stage->cell_h;
    x = 0;
  }
}

void draw(App *app, Stage *stage) { draw_cells(app, stage); }

Cell ***generate_seed(int rows, int cols) {
  Cell ***seed = calloc(rows, sizeof(Cell *));
  for (int row = 0; row < rows; row++) {
    seed[row] = calloc(cols, sizeof(Cell *));
  }

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      int isAlive = rand() % (10 + 1);

      seed[row][col] = (Cell *)calloc(1, sizeof(Cell));
      if (isAlive <= 3) {
        seed[row][col]->alive = true;
      } else {
        seed[row][col]->alive = false;
      }
    }
  }

  return seed;
}

Stage *init_stage(App *app, int rows, int cols) {
  app->delegate.logic = logic;
  app->delegate.draw = draw;

  Stage *stage = calloc(1, sizeof(Stage));
  stage->cell_w = CELL;
  stage->cell_h = CELL;
  stage->rows = rows;
  stage->cols = cols;
  stage->cells = generate_seed(rows, cols);

  return stage;
}

void free_stage(Stage *stage) {
  for (int row = 0; row < stage->rows; row++) {
    for (int col = 0; col < stage->cols; col++) {
      free(stage->cells[row][col]);
      stage->cells[row][col] = NULL;
    }
  }

  free(stage);
  stage = NULL;
}

