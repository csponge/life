#include "command.h"
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

void cell_grid_logic(void *element) {
	CellGrid *grid = (CellGrid *)element;
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

void draw(App *app, Stage *stage) {
	for (int i = 0; i < stage->num_elements; i++) {
		GuiElement *el = stage->elements[i];
		el->draw(app->renderer, el->element);
	}
}

void logic(App *app, Stage *stage) {
	for (int i = 0; i < stage->num_elements; i++) {
		GuiElement *el = stage->elements[i];
		if (el->logic != NULL) {
			el->logic(el->element);
		}
	}
}

void mouse_click(Stage *stage, int x, int y) {
	for (int i = 0; i < stage->num_elements; i++) {
		GuiElement *el = stage->elements[i];
		if (el->is_clicked == NULL)
			continue;

		if (el->is_clicked(el->element, x, y)) {
			ClickEventArgs args = {
			    .x = x, .y = y, .element = el->element};
			el->clicked(args);
		}
	}
}

void play_clicked(ClickEventArgs args) { 
    command_enqueue(Play); 
}

void cell_grid_clicked(ClickEventArgs args) {
	CellGrid *grid = (CellGrid *)args.element;
	int x = args.x;
	int y = args.y;

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

/*void seed(Stage *stage) {*/
/*	for (int row = 0; row < stage->grid.rows; row++) {*/
/*		for (int col = 0; col < stage->grid.cols; col++) {*/
/*			int isAlive = rand() % (10 + 1);*/
/**/
/*			stage->grid.cells[row][col] =*/
/*			    (Cell *)calloc(1, sizeof(Cell));*/
/*			if (isAlive <= 1) {*/
/*				stage->grid.cells[row][col]->alive = true;*/
/*			} else {*/
/*				stage->grid.cells[row][col]->alive = false;*/
/*			}*/
/*		}*/
/*	}*/
/*}*/

void add_elem_to_stage(Stage *stage, GuiElement *element) {
	stage->elements[stage->num_elements++] = element;
};

Stage *init_stage(App *app, int rows, int cols) {
	app->delegate.logic = logic;
	app->delegate.draw = draw;

	Stage *stage = calloc(1, sizeof(Stage));
	stage->elements = calloc(2, sizeof(GuiElement *));

	DrawInfo info = {.renderer = app->renderer, .font = app->font};

	// Create play button
	GuiElement *play = new_button(10, 10);
	button_set_text((Button *)play->element, &info, "Play");
	play->clicked = play_clicked;

	// assign gui element
	add_elem_to_stage(stage, play);

	// Create grid
	GuiElement *grid = new_cell_grid(rows, cols, 0, 60);
	grid->logic = cell_grid_logic;
	grid->clicked = cell_grid_clicked;
	add_elem_to_stage(stage, grid);

	return stage;
}

void free_stage(Stage *stage) {
	for (int i = 0; i < stage->num_elements; i++) {
		GuiElement *el = stage->elements[i];
		el->destroy(el->element);
	}

	free(stage);
	stage = NULL;
}
