#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef struct _App App;

typedef struct {
  bool alive;
  float x;
  float y;
} Cell;

typedef struct {
  int rows;
  int cols;
  int cell_h;
  int cell_w;
  Cell ***cells;
} Stage;

typedef struct {
  void (*logic)(App *app, Stage *stage);
  void (*draw)(App *app, Stage *stage);
} Delegate;

struct _App {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Delegate delegate;
  int keyboard[MAX_KEYBOAD_KEYS];
};

#endif
