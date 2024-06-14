#include "gui.h"
#include "common.h"
#include "defs.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

void draw_button(DrawInfo *info, void *button);
void draw_cell_grid(DrawInfo *info, void *grid);
void destroy_button(void *button);

GuiElement *new_element(void *element) {
	GuiElement *el = calloc(1, sizeof(GuiElement));
	if (el != NULL) {
		el->element = element;
	}

	return el;
}

GuiElement *new_button(float x, float y) {
	Button *btn = calloc(1, sizeof(Button));
	if (btn != NULL) {
		btn->x = x;
		btn->y = y;
	}

	GuiElement *el = calloc(1, sizeof(GuiElement));
	if (el != NULL) {
		el->element = btn;
		el->draw = draw_button;
        el->free = destroy_button;
	}

	return el;
}

GuiElement *new_cell_grid(int rows, int cols, float x, float y) {
    CellGrid *grid = calloc(1, sizeof(CellGrid));
    if (grid == NULL) {
        return NULL;
    }

    grid->x = x;
    grid->y = y;
    grid->rows = rows;
    grid->cols = cols;
    grid->cell_h = CELL;
    grid->cell_w = CELL;

	Cell ***cells = calloc(rows, sizeof(Cell *));
	for (int row = 0; row < rows; row++) {
		cells[row] = calloc(cols, sizeof(Cell *));
	}

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			cells[row][col] = (Cell *)calloc(1, sizeof(Cell));
			cells[row][col]->alive = false;
		}
	}
    grid->cells = cells;

    GuiElement *el = calloc(1, sizeof(GuiElement));
    if (el != NULL) {
        el->element = grid;
        el->draw = draw_cell_grid;
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

	int ret = 0;
	ret = SDL_SetRenderDrawColor(info->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if (ret != 0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
		return;
	}


	SDL_Surface *text =
	    TTF_RenderUTF8_Solid(info->font, btn->text, black);

	if (text == NULL) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to get rendered text: %s\n", SDL_GetError());
		return;
	}

	btn->texture = SDL_CreateTextureFromSurface(info->renderer, text);

	SDL_Rect dest = {
	    .x = btn->x,
	    .y = btn->y,
	};

    SDL_QueryTexture(btn->texture, NULL, NULL, &dest.w, &dest.h);

    SDL_LogInfo(SDL_LOG_PRIORITY_INFO, "button texture size -> w:%d h:%d\n", dest.w, dest.h);

	ret = SDL_RenderDrawRect(info->renderer, &dest);
	if (ret != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw button: %s", SDL_GetError());
		return;
	}

	SDL_RenderCopy(info->renderer, btn->texture, NULL, &dest);
}

void destroy_button(void *button) {
    Button *btn = (Button *)button;
    SDL_DestroyTexture(btn->texture);
	free(btn->text);
	free(btn);
}

void draw_cell(SDL_Renderer *renderer, Cell *cell, int w, int h) {
	SDL_Rect dest;

	dest.x = cell->x;
	dest.y = cell->y;
	dest.w = w;
	dest.h = h;

	// set the render color for all cells
	if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) !=
	    0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
	}

	if (SDL_RenderDrawRect(renderer, &dest) != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw rectangle: %s", SDL_GetError());
	}

	// only alive cells should be filled in
	if (cell->alive == true) {
		if (SDL_RenderFillRect(renderer, &dest) != 0) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			               SDL_LOG_PRIORITY_ERROR,
			               "failed to color rectangle: %s",
			               SDL_GetError());
		}
	}
}

void draw_cell_grid(DrawInfo *info, void *grid) {
    CellGrid *cgrid = (CellGrid *)grid;

	int x = cgrid->x;
	int y = cgrid->y;

	for (int row = 0; row < cgrid->rows; row++) {
		for (int col = 0; col < cgrid->cols; col++) {
			Cell *cell = cgrid->cells[row][col];
			cell->x = x;
			cell->y = y;

			draw_cell(info->renderer, cell, cgrid->cell_h, cgrid->cell_w);

			x += cgrid->cell_w;
		}
		y += cgrid->cell_h;
		x = 0;
	}
}

void free_cells(Cell ***cells, int rows, int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			free(cells[row][col]);
			cells[row][col] = NULL;
		}
	}

	free(cells);
	cells = NULL;
}


