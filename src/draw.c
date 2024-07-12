#include "draw.h"
#include "structs.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>

void prepare_scene(App *app) {
	SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
	SDL_RenderClear(app->renderer);
}

void present_scene(App *app) { SDL_RenderPresent(app->renderer); }
