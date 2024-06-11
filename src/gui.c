#include "gui.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

void draw_button(DrawInfo *info, void *button);

GuiElement *new_element(void *element) {
	GuiElement *el = calloc(1, sizeof(GuiElement));
	if (el != NULL) {
		el->element = element;
	}

	return el;
}

GuiElement *new_button(float x, float y, int w, int h) {
	Button *btn = calloc(1, sizeof(Button));
	if (btn != NULL) {
		btn->x = x;
		btn->y = y;
		btn->w = w;
		btn->h = h;
	}

	GuiElement *el = calloc(1, sizeof(GuiElement));
	if (el != NULL) {
		el->element = btn;
		el->draw = draw_button;
	}

	return el;
}

int button_set_text(Button *btn, char *text, size_t len) {
	btn->text = malloc(sizeof(char) * len);
	if (btn->text != NULL) {
		memcpy(btn->text, text, len);
		return len;
	}

	return -1;
}

void draw_button(DrawInfo *info, void *button) {
	Button *btn = (Button *)button;

	SDL_Rect dest = {
	    .x = btn->x,
	    .y = btn->y,
	    .w = btn->w,
	    .h = btn->h,
	};

	int ret = 0;

	ret = SDL_SetRenderDrawColor(info->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if (ret != 0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
		return;
	}

	ret = SDL_RenderDrawRect(info->renderer, &dest);
	if (ret != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw button: %s", SDL_GetError());
		return;
	}

	SDL_Color forecol = {0x00, 0x00, 0x00, 0};
	SDL_Color backcol = {0xFF, 0xFF, 0xFF, 0};
	SDL_Surface *text =
	    TTF_RenderUTF8_Solid(info->font, btn->text, forecol);

	if (text == NULL) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to get rendered text: %s\n", SDL_GetError());
		return;
	}

	btn->texture = SDL_CreateTextureFromSurface(info->renderer, text);

	SDL_RenderCopy(info->renderer, btn->texture, NULL, &dest);
}

void destroy_button(Button *btn) {
	free(btn->text);
	free(btn);
}
