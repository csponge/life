#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

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
  void (*mouse_click)(App *app, Stage *stage);
} Delegate;

typedef struct {
  bool clicked;
  int x;
  int y;
} Mouse;

struct _App {
  bool run;
  SDL_Renderer *renderer;
  SDL_Window *window;
  Delegate delegate;
  int keyboard[MAX_KEYBOAD_KEYS];
  Mouse mouse;
};

#endif
