#include "defs.h"
#include "draw.h"
#include "structs.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*static void do_player(App *app, Stage *stage, Entity *player,*/
/*                      SDL_Texture *bullet_texture) {*/
/*  player->dx = player->dy = 0;*/
/**/
/*  if (player->reload > 0) {*/
/*    player->reload--;*/
/*  }*/
/**/
/*  if (app->keyboard[SDL_SCANCODE_UP]) {*/
/*    player->dy = player->y - PLAYER_SPEED >= 0 ? -PLAYER_SPEED : -player->y;*/
/*  }*/
/**/
/*  if (app->keyboard[SDL_SCANCODE_DOWN]) {*/
/*    player->dy = player->y + player->h + PLAYER_SPEED <= SCREEN_HEIGHT*/
/*                     ? PLAYER_SPEED*/
/*                     : SCREEN_HEIGHT - (player->y + player->h);*/
/*  }*/
/**/
/*  if (app->keyboard[SDL_SCANCODE_LEFT]) {*/
/*    player->dx = player->x - PLAYER_SPEED >= 0 ? -PLAYER_SPEED : -player->x;*/
/*  }*/
/**/
/*  if (app->keyboard[SDL_SCANCODE_RIGHT]) {*/
/*    player->dx = player->x + player->w + PLAYER_SPEED <= SCREEN_WIDTH*/
/*                     ? PLAYER_SPEED*/
/*                     : SCREEN_WIDTH - (player->x + player->w);*/
/*  }*/
/**/
/*  if (app->keyboard[SDL_SCANCODE_F] && player->reload == 0) {*/
/*  }*/
/**/
/*  player->x += player->dx;*/
/*  player->y += player->dy;*/
/*}*/

void logic(App *app, Stage *stage) {}

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
  stage->cell_w = 32;
  stage->cell_h = 32;
  stage->rows = rows;
  stage->cols = cols;
  stage->cells = generate_seed(rows, cols);

  return stage;
}
