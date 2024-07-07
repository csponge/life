#include "structs.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdlib.h>

int generation = 0;

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

int count_live_neighbors(CellGrid *grid, int row, int col) {
	int live_neighbors = 8;

	// top
	if ((row > 0 && col > 0 ? grid->cells[row - 1][col - 1]->alive
	                        : false) == false)
		live_neighbors--;

	if ((row > 0 ? grid->cells[row - 1][col]->alive : false) == false)
		live_neighbors--;

	if ((row > 0 && col < (grid->cols - 1)
	         ? grid->cells[row - 1][col + 1]->alive
	         : false) == false)
		live_neighbors--;

	// middle
	if ((col > 0 ? grid->cells[row][col - 1]->alive : false) == false)
		live_neighbors--;

	if ((col < (grid->cols - 1) ? grid->cells[row][col + 1]->alive
	                            : false) == false)
		live_neighbors--;

	// bottom
	if ((row < (grid->rows - 1) && col > 0
	         ? grid->cells[row + 1][col - 1]->alive
	         : false) == false)
		live_neighbors--;

	if ((row < (grid->rows - 1) ? grid->cells[row + 1][col]->alive
	                            : false) == false)
		live_neighbors--;

	if ((row < (grid->rows - 1) && col < (grid->cols - 1)
	         ? grid->cells[row + 1][col + 1]->alive
	         : false) == false)
		live_neighbors--;

	return live_neighbors;
}

void seed(CellGrid *grid) {
	for (int row = 0; row < grid->rows; row++) {
		for (int col = 0; col < grid->cols; col++) {
			int isAlive = rand() % (10 + 1);

			grid->cells[row][col] = (Cell *)calloc(1, sizeof(Cell));
			if (isAlive <= 1) {
				grid->cells[row][col]->alive = true;
			} else {
				grid->cells[row][col]->alive = false;
			}
		}
	}
}

void next_generation(CellGrid *grid) {
	Cell ***new_cells = initialize_cells(grid->rows, grid->cols);

	for (int row = 0; row < grid->rows; row++) {
		for (int col = 0; col < grid->cols; col++) {
			int live_neighbors =
			    count_live_neighbors(grid, row, col);

			// handle dead cells
			if (grid->cells[row][col]->alive == false) {
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

	free_cells(grid->cells, grid->rows, grid->cols);
	grid->cells = new_cells;
}

void draw(Stage *stage, SDL_Renderer *renderer) {
	button_blit(stage->play_btn, renderer);
	/* button_blit(stage->pause_btn, renderer); */
	/* button_blit(stage->seed_btn, renderer); */
	/* button_blit(stage->dec_tick_btn, renderer); */
	/* button_blit(stage->inc_tick_btn, renderer); */
	cell_grid_blit(stage->cell_grid, renderer);
}

void logic(App *app, Stage *stage) { next_generation(stage->cell_grid); }

void cell_grid_clicked(CellGrid *grid, int x, int y) {
	int found_row = -1;
	int found_col = -1;

	// get row
	for (int row = 0; row < grid->rows; row++) {
		int spix = grid->y + (row * CELL);
		int epix = spix + CELL;

		if (y > spix && y < epix) {
			found_row = row;
			break;
		}
	}

	// get col
	for (int col = 0; col < grid->cols; col++) {
		int spix = (col * CELL);
		int epix = spix + CELL;

		if (x > spix && x < epix) {
			found_col = col;
			break;
		}
	}

	if (found_col != -1 && found_row != -1) {
		grid->cells[found_row][found_col]->alive =
		    !grid->cells[found_row][found_col]->alive;
	}
}

void mouse_click(App *app, Stage *stage, int x, int y) {
	bool clicked = false;

	clicked = is_button_clicked(stage->play_btn, x, y);
	if (clicked) {
		app->run = true;
	}

	clicked = is_button_clicked(stage->pause_btn, x, y);
	if (clicked) {
		app->run = false;
	}

	clicked = is_button_clicked(stage->seed_btn, x, y);
	if (clicked) {
		seed(stage->cell_grid);
	}

	clicked = is_button_clicked(stage->dec_tick_btn, x, y);
	if (clicked) {
		if (app->logic_tick > 0)
			app->logic_tick -= 10;
	}

	clicked = is_button_clicked(stage->inc_tick_btn, x, y);
	if (clicked) {
		app->logic_tick += 10;
	}

	clicked = is_cell_grid_clicked(stage->cell_grid, x, y);
	if (clicked) {
		cell_grid_clicked(stage->cell_grid, x, y);
	}
}

Stage *init_stage(App *app, int rows, int cols) {
	app->delegate.logic = logic;
	app->delegate.draw = draw;

	Stage *stage = calloc(1, sizeof(Stage));

	DrawInfo info = {.renderer = app->renderer, .font = app->font};
	Options opts = {.radius = 5};

	Button *play = new_button(10, 10, opts);
	button_set_text(play, &info, "Play");
	stage->play_btn = play;

	Button *pause = new_button(80, 10, opts);
	button_set_text(pause, &info, "Pause");
	stage->pause_btn = pause;

	Button *seed = new_button(170, 10, opts);
	button_set_text(seed, &info, "Seed");
	stage->seed_btn = seed;

	Button *dec_tick = new_button(245, 10, opts);
	button_set_text(dec_tick, &info, "Dec");
	stage->dec_tick_btn = dec_tick;

	Button *inc_tick = new_button(310, 10, opts);
	button_set_text(inc_tick, &info, "Inc");
	stage->inc_tick_btn = inc_tick;

	int cell_start = SCREEN_HEIGHT - (rows * CELL);
	CellGrid *grid = new_cell_grid(rows, cols, 0, cell_start);
	stage->cell_grid = grid;

	return stage;
}

void free_stage(Stage *stage) {
	button_destroy(stage->play_btn);
	button_destroy(stage->pause_btn);
	button_destroy(stage->dec_tick_btn);
	button_destroy(stage->inc_tick_btn);
	cell_grid_destroy(stage->cell_grid);

	free(stage);
	stage = NULL;
}
