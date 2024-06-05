#include "structs.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdlib.h>

Cell ***initialize_cells(int rows, int cols) {
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

	return cells;
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

int count_live_neighbors(Stage *stage, int row, int col) {
	int live_neighbors = 8;

	// top
	if ((row > 0 && col > 0 ? stage->grid.cells[row - 1][col - 1]->alive
	                        : false) == false)
		live_neighbors--;

	if ((row > 0 ? stage->grid.cells[row - 1][col]->alive : false) == false)
		live_neighbors--;

	if ((row > 0 && col < (stage->grid.cols - 1)
	         ? stage->grid.cells[row - 1][col + 1]->alive
	         : false) == false)
		live_neighbors--;

	// middle
	if ((col > 0 ? stage->grid.cells[row][col - 1]->alive : false) == false)
		live_neighbors--;

	if ((col < (stage->grid.cols - 1)
	         ? stage->grid.cells[row][col + 1]->alive
	         : false) == false)
		live_neighbors--;

	// bottom
	if ((row < (stage->grid.rows - 1) && col > 0
	         ? stage->grid.cells[row + 1][col - 1]->alive
	         : false) == false)
		live_neighbors--;

	if ((row < (stage->grid.rows - 1)
	         ? stage->grid.cells[row + 1][col]->alive
	         : false) == false)
		live_neighbors--;

	if ((row < (stage->grid.rows - 1) && col < (stage->grid.cols - 1)
	         ? stage->grid.cells[row + 1][col + 1]->alive
	         : false) == false)
		live_neighbors--;

	return live_neighbors;
}

void logic(App *app, Stage *stage) {
	if (app->run == false)
		return;

	Cell ***new_cells =
	    initialize_cells(stage->grid.rows, stage->grid.cols);

	for (int row = 0; row < stage->grid.rows; row++) {
		for (int col = 0; col < stage->grid.cols; col++) {
			int live_neighbors =
			    count_live_neighbors(stage, row, col);

			// handle dead cells
			if (stage->grid.cells[row][col]->alive == false) {
				if (live_neighbors == 3) {
					new_cells[row][col]->alive = true;
				}
				continue;
			}

			// handle alive cells
			if (live_neighbors < 2 || live_neighbors > 3) {
				new_cells[row][col]->alive = false;
			} else {
				new_cells[row][col]->alive = true;
			}
		}
	}

	free_cells(stage->grid.cells, stage->grid.rows, stage->grid.cols);
	stage->grid.cells = new_cells;
}

void draw_cell(App *app, Cell *cell, int w, int h) {
	SDL_Rect dest;

	dest.x = cell->x;
	dest.y = cell->y;
	dest.w = w;
	dest.h = h;

	// set the render color for all cells
	if (SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) !=
	    0) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to set the render color: %s", SDL_GetError());
	}

	if (SDL_RenderDrawRect(app->renderer, &dest) != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw rectangle: %s", SDL_GetError());
	}

	// only alive cells should be filled in
	if (cell->alive == true) {
		if (SDL_RenderFillRect(app->renderer, &dest) != 0) {
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			               SDL_LOG_PRIORITY_ERROR,
			               "failed to color rectangle: %s",
			               SDL_GetError());
		}
	}
}

void draw_toolbar(App *app, Toolbar *toolbar) {
	SDL_Rect dest;

	dest.x = toolbar->play_button->x;
	dest.y = toolbar->play_button->y;
	dest.w = toolbar->play_button->w;
	dest.h = toolbar->play_button->h;

	if (SDL_RenderDrawRect(app->renderer, &dest) != 0) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		               SDL_LOG_PRIORITY_ERROR,
		               "failed to draw button: %s", SDL_GetError());
		return;
	}

	SDL_Surface *text = NULL;
	SDL_Color forecol = {0x00, 0x00, 0x00, 0};
	SDL_Color backcol = {0xFF, 0xFF, 0xFF, 0};

	text = TTF_RenderUTF8_Solid(app->font, toolbar->play_button->text,
	                            forecol);

	if (text == NULL) {
		SDL_LogMessage(
		    SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		    "failed to get rendered text: %s\n", SDL_GetError());
		return;
	}

	toolbar->play_button->texture =
	    SDL_CreateTextureFromSurface(app->renderer, text);

	SDL_RenderCopy(app->renderer, toolbar->play_button->texture, NULL,
	               &dest);
}

void draw_cells(App *app, CellGrid *grid) {
	int x = grid->x;
	int y = grid->y;

	for (int row = 0; row < grid->rows; row++) {
		for (int col = 0; col < grid->cols; col++) {
			Cell *cell = grid->cells[row][col];
			cell->x = x;
			cell->y = y;

			draw_cell(app, cell, grid->cell_h, grid->cell_w);

			x += grid->cell_w;
		}
		y += grid->cell_h;
		x = 0;
	}
}

void draw(App *app, Stage *stage) {
	draw_cells(app, &stage->grid);
	draw_toolbar(app, &stage->toolbar);
}

void mouse_click(App *app, Stage *stage) {
	if (app->mouse.clicked == false)
		return;

	int x = app->mouse.x;
	int y = app->mouse.y;
	int found_row = -1;
	int found_col = -1;

	// check if play pressed
	Button *play = stage->toolbar.play_button;
	if (x > play->x && x < (play->x + play->w) && y > play->y &&
	    y < (play->y + play->h)) {
        app->run = true;
	}

	// get row
	for (int row = 0; row < stage->grid.rows; row++) {
		int spix = stage->grid.y + (row * CELL);
		int epix = spix + CELL;

		if (y > spix && y < epix) {
			found_row = row;
			break;
		}
	}

	// get col
	for (int col = 0; col < stage->grid.cols; col++) {
		int spix = (col * CELL);
		int epix = spix + CELL;

		if (x > spix && x < epix) {
			found_col = col;
			break;
		}
	}

	if (found_col != -1 && found_row != -1) {
		stage->grid.cells[found_row][found_col]->alive = true;
	}
}

void seed(Stage *stage) {
	for (int row = 0; row < stage->grid.rows; row++) {
		for (int col = 0; col < stage->grid.cols; col++) {
			int isAlive = rand() % (10 + 1);

			stage->grid.cells[row][col] =
			    (Cell *)calloc(1, sizeof(Cell));
			if (isAlive <= 1) {
				stage->grid.cells[row][col]->alive = true;
			} else {
				stage->grid.cells[row][col]->alive = false;
			}
		}
	}
}

Stage *init_stage(App *app, int rows, int cols) {
	app->delegate.logic = logic;
	app->delegate.draw = draw;
	app->delegate.mouse_click = mouse_click;

	Stage *stage = calloc(1, sizeof(Stage));
	stage->grid.x = 0;
	stage->grid.y = 80;
	stage->grid.cell_w = CELL;
	stage->grid.cell_h = CELL;
	stage->grid.rows = rows;
	stage->grid.cols = cols;
	stage->grid.cells = initialize_cells(rows, cols);

	Button *play = calloc(1, sizeof(Button));
	play->x = 10;
	play->y = 10;
	play->w = 80;
	play->h = 40;
	play->text = "Play";
	stage->toolbar.play_button = play;

	return stage;
}

void free_stage(Stage *stage) {
	free_cells(stage->grid.cells, stage->grid.rows, stage->grid.cols);
	free(stage);
	stage = NULL;
}
