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
  Entity fighterHead, *fighterTail;
  Entity bulletHead, *bulletTail;
} Stage;

typedef struct {
  void (*logic)(App *app, Stage *stage, Entity *player, SDL_Texture *bullet_texture);
  void (*draw)(App *app, Stage *stage, Entity *player);
} Delegate;

struct _App {
  SDL_Renderer *renderer;
  SDL_Window *window;
  Delegate delegate;
  int keyboard[MAX_KEYBOAD_KEYS];
};

#endif
