#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct _Button Button;
typedef struct _clickEventArgs ClickEventArgs;

struct _Button {
	SDL_Rect rect;
	SDL_Texture *texture;
};

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

typedef struct _guiElement {
	void *element;
	void (*draw)(SDL_Renderer *renderer, void *element);
	bool (*is_clicked)(void *element, int x, int y);
	void (*clicked)(ClickEventArgs args);
    void (*logic)(void *element);
	void (*destroy)(void *element);
} GuiElement;

struct _clickEventArgs {
    int x;
    int y;
    GuiElement *element;
};

/**
 * Creates a new `GuiElement` struct of type `Button`.
 * Returns NULL if failed.
 * */
GuiElement *new_button(int x, int y);

/**
 * Creates a new `GuiElement` struct of type `Grid`.
 * Returns NULL if failed.
 * */
GuiElement *new_cell_grid(int rows, int cols, int x, int y);

/**
 * Copies the text to the button struct. Returns the number
 * of bytes written. Returns -1 if failed.
 * */
int button_set_text(Button *btn, DrawInfo *info, char *text);

#endif
