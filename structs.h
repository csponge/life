#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef struct _App App;

typedef struct Entity {
  float x;
  float y;
  int w;
  int h;
  float dx;
  float dy;
  int health;
  int reload;
  SDL_Texture *texture;
  struct Entity *next;
} Entity;

typedef struct {
  bool alive;
  int w;
  int h;
  float x;
  float y;
} Cell;

typedef struct {
  Cell **cells;
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
