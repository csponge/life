#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef struct _App App;

typedef struct {
  bool alive;
  float x;
  float y;
} Cell;

typedef struct {
  float x;
  float y;
  int w;
  int h;
  char *text;
} Button;

typedef struct {
  Button *play_button;
} Toolbar;

typedef struct {
  float x;
  float y;
  int rows;
  int cols;
  int cell_h;
  int cell_w;
  Cell ***cells;
} CellGrid;

typedef struct {
  Toolbar toolbar;
  CellGrid grid;
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
  TTF_Font *font;
  SDL_Window *window;
  Delegate delegate;
  int keyboard[MAX_KEYBOAD_KEYS];
  Mouse mouse;
};

#endif
