#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct _btn {
	SDL_Rect rect;
	SDL_Texture *texture;
} Button;

typedef struct _tb {
    SDL_Rect rect;
    SDL_Texture *texture;
} TextBox;

typedef struct {
	bool alive;
	float x;
	float y;
} Cell;

typedef struct {
	int x;
	int y;
	int rows;
	int cols;
	int cell_h;
	int cell_w;
	Cell ***cells;
} CellGrid;

typedef struct {
	SDL_Renderer *renderer;
	TTF_Font *font;
} DrawInfo;


/**
 * Creates a new `Grid`. Returns NULL if failed.
 * */
CellGrid *new_cell_grid(int rows, int cols, int x, int y);

/**
 * Checkes if the click was in the bounds of the provided
 * cell grid.
 * */
bool is_cell_grid_clicked(CellGrid *grid, int x, int y);

/**
 * Draw the provided cell grid on the screen.
 * */
void cell_grid_blit(CellGrid *grid, SDL_Renderer *renderer);

/**
 * Free the cell grid memory.
 * */
void cell_grid_destroy(CellGrid *grid);

/**
 * Creates a new `TextBox`. Returns NULL if failed.
 * */
TextBox *new_text_box(int x, int y);

/**
 * Copies the text to the text box struct. Returns the number
 * of bytes written. Returns -1 if failed.
 * */
int text_box_set_text(TextBox *tb, DrawInfo *info, char *text);

/**
 * Draw the provided text box on the screen.
 * */
void text_box_blit(TextBox *tb, SDL_Renderer *renderer);

/**
 * Free the text box memory.
 * */
void text_box_destroy(TextBox *tb);

/**
 * Creates a new `Button`. Returns NULL if failed.
 * */
Button *new_button(int x, int y);

/**
 * Copies the text to the button struct. Returns the number
 * of bytes written. Returns -1 if failed.
 * */
int button_set_text(Button *btn, DrawInfo *info, char *text);

/**
 * Checkes if the click was in the bounds of the provided
 * button.
 * */
bool is_button_clicked(Button *btn, int x, int y);

/**
 * Draw the provided button on the screen.
 * */
void button_blit(Button *btn, SDL_Renderer *renderer);

/**
 * Free the button memory.
 * */
void button_destroy(Button *btn);

#endif
