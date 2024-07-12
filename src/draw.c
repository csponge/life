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

SDL_Texture *load_texture(App *app, char *filename) {
	SDL_Texture *texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
	               "Loading %s", filename);
	texture = IMG_LoadTexture(app->renderer, filename);
	return texture;
}

void get_texture_rect(SDL_Texture *texture, int *w, int *h) {
	SDL_QueryTexture(texture, NULL, NULL, w, h);
}

void blit(App *app, SDL_Texture *texture, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(app->renderer, texture, NULL, &dest);
}
