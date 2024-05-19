#include "defs.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

App init_sdl(void) {
  int render_flags, window_flags;

  render_flags = SDL_RENDERER_ACCELERATED;
  window_flags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  App app = {.window = SDL_CreateWindow("Life", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, window_flags)};

  if (!app.window) {
    printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  app.renderer = SDL_CreateRenderer(app.window, -1, render_flags);

  if (!app.renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
    printf("Couldn't initialize SDL Image library: %s\n", SDL_GetError());
    exit(1);
  }

  return app;
}
