#include "gui.h"
#include "common.h"
#include "defs.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

void draw_cell_grid(SDL_Renderer *renderer, void *grid);
void destroy_cell_grid(void *cgrid);

void button_blit(Button *btn, SDL_Renderer *renderer) {
	int ret = 0;
	ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if (ret != 0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
		return;
	}

	ret = SDL_RenderDrawRect(renderer, &btn->rect);
	if (ret != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw button: %s", SDL_GetError());
		return;
	}

	SDL_RenderCopy(renderer, btn->texture, NULL, &btn->rect);
}

Button *new_button(int x, int y) {
	Button *btn = calloc(1, sizeof(Button));
	if (btn != NULL) {
		btn->rect.x = x;
		btn->rect.y = y;
	}

	return btn;
}


int button_set_text(Button *btn, DrawInfo *info, char *text) {
	SDL_Surface *text_surf = TTF_RenderUTF8_Solid(info->font, text, black);

	if (text_surf == NULL) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to get rendered text: %s\n", SDL_GetError());
		return 1;
	}

	btn->texture = SDL_CreateTextureFromSurface(info->renderer, text_surf);

	SDL_QueryTexture(btn->texture, NULL, NULL, &btn->rect.w, &btn->rect.h);

	return 0;
}

bool is_button_clicked(Button *btn, int x, int y) {
	if (btn->rect.y > y || (btn->rect.y + btn->rect.h) < y) {
		return false;
	}

	if (btn->rect.x > x || (btn->rect.x + btn->rect.w) < x) {
		return false;
	}

	return true;
}

void button_destroy(Button *btn) {
	SDL_DestroyTexture(btn->texture);
	free(btn);
}

void text_box_blit(TextBox *tb, SDL_Renderer *renderer) {
	int ret = 0;
	ret = SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	if (ret != 0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
		return;
	}

	SDL_RenderCopy(renderer, tb->texture, NULL, &tb->rect);
}

void text_box_destroy(TextBox *tb) {
	SDL_DestroyTexture(tb->texture);
	free(tb);
}

TextBox *new_text_box(int x, int y) {
	TextBox *tb = calloc(1, sizeof(TextBox));
	if (tb != NULL) {
		tb->rect.x = x;
		tb->rect.y = y;
	}

	return tb;
}

int text_box_set_text(TextBox *tb, DrawInfo *info, char *text) {
	SDL_Surface *text_surf = TTF_RenderUTF8_Solid(info->font, text, red);

	if (text_surf == NULL) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to get rendered text: %s\n", SDL_GetError());
		return 1;
	}

	tb->texture = SDL_CreateTextureFromSurface(info->renderer, text_surf);

	SDL_QueryTexture(tb->texture, NULL, NULL, &tb->rect.w, &tb->rect.h);

	return 0;
}

bool is_cell_grid_clicked(CellGrid *grid, int x, int y) {
	return x >= grid->x && y >= grid->y ? true : false;
}

CellGrid *new_cell_grid(int rows, int cols, int x, int y) {
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

	return grid;
}

void draw_cell(SDL_Renderer *renderer, Cell *cell, int w, int h) {
	SDL_Rect dest;

	dest.x = cell->x;
	dest.y = cell->y;
	dest.w = w;
	dest.h = h;

	// set the render color for all cells
	if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0) {
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

void cell_grid_blit(CellGrid *grid, SDL_Renderer *renderer) {
	int x = grid->x;
	int y = grid->y;

	for (int row = 0; row < grid->rows; row++) {
		for (int col = 0; col < grid->cols; col++) {
			Cell *cell = grid->cells[row][col];
			cell->x = x;
			cell->y = y;

			draw_cell(renderer, cell, grid->cell_h, grid->cell_w);

			x += grid->cell_w;
		}
		y += grid->cell_h;
		x = 0;
	}
}

void destroy_cells(Cell ***cells, int rows, int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			free(cells[row][col]);
			cells[row][col] = NULL;
		}
	}

	free(cells);
	cells = NULL;
}

void cell_grid_destroy(CellGrid *grid) {
	destroy_cells(grid->cells, grid->rows, grid->cols);
	free(grid);
}
